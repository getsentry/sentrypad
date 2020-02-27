import pytest
import subprocess
import os
import re
import sys
from . import cmake

def enumerate_unittests():
    regexp = re.compile("XX\((.*?)\)")
    tests = []
    # TODO: actually generate the `tests.inc` file with python
    with open('tests/unit/tests.inc', 'r') as testsfile:
        for line in testsfile:
            match = regexp.match(line)
            if match:
                tests.append(match.group(1))
    return tests

def pytest_generate_tests(metafunc):
    if "unittest" in metafunc.fixturenames:
        metafunc.parametrize("unittest", enumerate_unittests())


class Unittests:
    def __init__(self, dir):
        # for unit tests, the backend does not matter, and we want to keep
        # the compile-times down
        cmake(dir, ["sentry_test_unit"], ["SENTRY_BACKEND=none"])
        self.dir = dir
    def run(self, test):
        subprocess.run(["./sentry_test_unit" if sys.platform != "win32" else "sentry_test_unit.exe", test], cwd=self.dir, check=True)

@pytest.fixture(scope="session")
def unittests(tmp_path_factory):
    tmpdir = tmp_path_factory.mktemp("unittests")
    return Unittests(tmpdir)
