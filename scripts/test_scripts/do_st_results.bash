#!/bin/bash
# $Id: do_st_results.bash,v 1.1.2.1 2004/07/06 15:11:43 svasquez Exp $
# This script runs at the end of the "run_system_tests" target.
# The purpose is to give the user the results of running the system_tests.

echo ""
echo "The following is the analysis of the run system tests results:"
echo ""

export dir=$ESMF_DIR/test/test$ESMF_BOPT/$ESMF_ARCH.$ESMF_COMPILER.$ESMF_PREC.$ESMF_SITE


find $dir -name "*STest" -print | wc -l > stdoutfiles
if grep -q " 0" stdoutfiles # check if there are any executable files 
then
	echo "There are no executable system tests file, either the 'gmake build_system_tests' has "
	echo "not been run or the 'build_system_tests' did not build successfully."
	echo "" 
else
	echo "Executable System Tests found:"
	(cd $dir; ls -l *STest)
	echo "" 
	echo "All of the executable system tests should have a corresponding stdout file." 
	echo "If not, it's an indication that the examples ere not executed, or that it failed to execute." 
	echo "" 

fi # end of if grep " 0" stdoutfiles

find $dir -name "*STest.stdout" -print | wc -l > stdoutfiles
if grep -q " 0" stdoutfiles 
then
	echo "There are no system tests stdout files." 
	echo "" 
else
	echo "System Tests stdout files found: " 
	(cd $dir; ls -l *STest.stdout)
	echo "" 
	echo "System Tests stdout files of zero length indicate that the test" 
	echo "did not run because it failed to compile or it failed to execute. " 
	echo "" 
	if grep -q "PASS" $dir/*STest.stdout 
	then
		echo "The following tests passed: "  
		(cd $dir; grep -l "PASS" *STest.stdout)
		echo "" 
 	else
		echo "No system tests passed." 
		echo "" 
	fi #end of if grep PASS ...

        echo ""
	if grep -q "FAIL:" $dir/*STest.stdout
	then
		echo "The following tests failed: "  
		(cd $dir; grep -l "FAIL:" *STest.stdout)
		echo "" 
	else
		find $dir -name "*STest.stdout" -size 0 -print | wc -l > zerofiles
                #check if all the stdoutfiles are of zero length
		if diff stdoutfiles zerofiles > NULL
		then
			echo ""
		else
			echo "No system tests Failed." 
			echo "" 

		fi #end of if diff stdoutfiles zerofiles

	fi #end of if grep FAIL $dir/*STest.stdout

fi #end of if grep " 0" stdoutfiles
rm -f stdoutfiles
rm -f zerofiles
rm -f NULL

