#!/bin/bash
#		2_43_test.sh

while read a b c d e ; do
	res=`echo $a $b $c $d | ../solves/2_43`
	if [ x"$e" != x"$res" ]; then
		echo TEST $a $b $c $d FAILED: expected "$e" got "$res"
	fi
done <<END
	12 34 56 78 `1357\n2468\n`
	ab cd ef gh `aceg\nbdfh\n`
	tYo poE GAq 2Pa `tpG2\nYoAP\noEqa2\n`
	abcd ea RRRRR q `aeRq\nbaR \nc R \nd R \n  R \n`
END
