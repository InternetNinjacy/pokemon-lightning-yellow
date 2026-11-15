"""Evaluate the actual review-event guard; no review or workflow is submitted."""
import ast
from copy import deepcopy
from pathlib import Path
import subprocess
from types import SimpleNamespace as NS
import unittest

import yaml

ROOT = Path(__file__).resolve().parents[1]
WORKFLOW = ".github/workflows/wrl001-supplemental.yml"
DOC = yaml.load((ROOT / WORKFLOW).read_text(), Loader=yaml.BaseLoader)
EXPR = DOC["jobs"]["supplemental-wrl001"]["if"].replace("&&", " and ")
TREE = ast.parse(EXPR, mode="eval")
for node in ast.walk(TREE):
    if not isinstance(node, (ast.Expression, ast.BoolOp, ast.And, ast.Compare,
                             ast.Eq, ast.Attribute, ast.Name, ast.Load, ast.Constant)):
        raise AssertionError(f"Unexpected guard syntax: {type(node).__name__}")
CODE = compile(TREE, WORKFLOW, "eval")
REPO = "InternetNinjacy/pokemon-lightning-yellow"


def event():
    return NS(event_name="pull_request_review", repository=REPO,
              actor="InternetNinjacy", event=NS(
                  review=NS(user=NS(login="InternetNinjacy"), state="commented",
                            body="RUN WRL001 ONLY 0f693dd7bef4219e161b299f08a9a225ecdac9d6",
                            commit_id="reviewed-head"),
                  pull_request=NS(number=250, state="open", head=NS(
                      sha="reviewed-head", repo=NS(full_name=REPO),
                      ref="proposal/wrl001-exact-commit-ci-20261115"))))


def accepted(context):
    return eval(CODE, {"__builtins__": {}}, {"github": context})


class ReviewTriggerTests(unittest.TestCase):
    def test_exact_owner_review_is_accepted(self):
        self.assertTrue(accepted(event()))

    def test_each_nonmatching_review_is_rejected(self):
        cases = {
            "event_name": "pull_request",
            "repository": "another/repository",
            "actor": "another-user",
            "event.review.user.login": "another-user",
            "event.review.state": "approved",
            "event.review.body": "ordinary review text",
            "event.review.commit_id": "older-head",
            "event.pull_request.number": 249,
            "event.pull_request.state": "closed",
            "event.pull_request.head.repo.full_name": "another/fork",
            "event.pull_request.head.ref": "another-branch",
        }
        for path, value in cases.items():
            with self.subTest(path=path):
                context = deepcopy(event())
                target = context
                parts = path.split(".")
                for key in parts[:-1]:
                    target = getattr(target, key)
                setattr(target, parts[-1], value)
                self.assertFalse(accepted(context))

    def test_only_review_submission_and_read_permissions(self):
        self.assertEqual(DOC["on"], {"pull_request_review": {"types": ["submitted"]}})
        self.assertEqual(DOC["permissions"], {"contents": "read"})
        steps = DOC["jobs"]["supplemental-wrl001"]["steps"]
        self.assertEqual(steps[2]["with"]["ref"], "0f693dd7bef4219e161b299f08a9a225ecdac9d6")
        self.assertFalse(any("upload-artifact" in s.get("uses", "") or
                             "actions/cache" in s.get("uses", "") for s in steps))

    def test_every_existing_workflow_is_unchanged(self):
        files = subprocess.check_output(["git", "ls-tree", "-r", "--name-only", "origin/main",
                                         ".github/workflows"], cwd=ROOT, text=True).splitlines()
        self.assertEqual(len(files), 7)
        for name in files:
            before = subprocess.check_output(["git", "show", f"origin/main:{name}"], cwd=ROOT)
            self.assertEqual((ROOT / name).read_bytes(), before, name)
            existing = yaml.load(before, Loader=yaml.BaseLoader)
            self.assertNotIn("pull_request_review", existing["on"], name)


if __name__ == "__main__":
    unittest.main()
