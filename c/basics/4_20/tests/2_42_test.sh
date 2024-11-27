#!/bin/sh
#		2_42_test.sh

while read a b c d ; do
	res=`echo $a $b $c $d | ../tasks/2_42`
	if [ x"$d $c $b $a" != x"$res" ]; then
		echo TEST \"$a $b $c $d\" FAILED: expected \""$d $c $b $a"\", got \""$res"\"
	fi
done <<END
	a b c d
	test1 test2 test3 test4
	Happy New year everyone
	d b c a
	abra shwabra kada bra
	supermega longstring andtoomuchcharachters test
END
