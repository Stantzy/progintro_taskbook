#!/bin/bash
#		2_44_test.sh

answer1="12 34 56 78"
answer2="1"
answer3="567"
answer4="11111"

while read a b c d e ; do
	res=`echo $a $b $c $d | \.\./solves/2_44`
	if [ x"$e" != x"$res" ]; then
		echo TEST $a $b $c $d FAILED: expected "$e" got "$res"
	fi
done <<END
	12 34 56 78 $answer1
	ab c1d ef gh $answer2
	tY1o po567E GAq 22Pa $answer3
	abcd ea 11111 q $answer4
END