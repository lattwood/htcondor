import datetime
import errno
import htcondor
import os
import subprocess
import sys
import time

from PersonalCondor import PersonalCondor
from Utils import Utils

JOB_SUCCESS = 0
JOB_FAILURE = 1

TEST_SUCCESS = 0
TEST_FAILURE = 1


class CondorTest(object):

    def __init__(self, name, params=None):
        self._name = name
        self._params = params
        self._personal_condors = dict()


    def __del__(self):
        self.End()


    def FailureCallback(self):
        Utils.TLog("Default CondorTest failure callback invoked")


    def SubmitCallback(self):
        Utils.TLog("Default CondorTest submit callback invoked")


    def SuccessCallback(self):
        Utils.TLog("Default CondorTest success callback invoked")


    # @return: -1 if error, or the numeric key >=0 of this PersonalCondor instance if success
    def StartPersonalCondor(self):
        personal = PersonalCondor(self._name, self._params)
        success = personal.Start()
        if success is not True:
            return -1
        self._personal_condors[0] = personal
        return 0


    def SubmitJob(self, job, wait=True, key=0):

        self.SubmitCallback()

        result = JOB_FAILURE
        
        # If no PersonalCondor objects are registered to this test, submit to system condor
        if len(self._personal_condors) == 0:
            result = job.Submit(wait)
        # If a single PersonalCondor object is registered, update the system CONDOR_CONFIG 
        # to point to that object's condor_config.local, then submit the test
        elif len(self._personal_condors) == 1:
            for key, personal in self._personal_condors.items():
                personal.SetCondorConfig()
                result = job.Submit(wait)
        # If multiple PersonalCondor objects are registered, check the user-supplied key
        # value, update CONDOR_CONFIG to point to the specific instance, then submit the test
        elif len(self._personal_condors) > 1:
            self._personal_condors[key].SetCondorConfig()
            result = job.Submit(wait)

        # Invoke the appropriate callback function
        Utils.TLog("Submit returned result = " + str(result))
        if result == JOB_SUCCESS:
            self.SuccessCallback()
        elif result == JOB_FAILURE:
            self.FailureCallback()


    def RegisterSubmit(self, submit_callback_fn):
        self.SubmitCallback = submit_callback_fn


    def RegisterSuccess(self, success_callback_fn):
        self.SuccessCallback = success_callback_fn


    def RegisterFailure(self, failure_callback_fn):
        self.FailureCallback = failure_callback_fn


    def End(self):
        try:
            for key, personal in self._personal_condors.items():
                personal.Stop()
                del self._personal_condors[key]
        except:
            Utils.TLog("PersonalCondor was not running, ending test")
