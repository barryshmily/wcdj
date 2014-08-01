#!/bin/bash


if [ $# != 2 ]; then
	echo "usage: xxx.sh [-s/-t] xxx"
	exit 0
fi

case "$1" in
	-s)
	mysql -uroot -e "use portal_conf;select * from portal_service_config where Fservice_code=\"$2\" limit 1\G"
	if [ $? != 0 ]; then 
		echo "servicecode error" 
	fi

	;;

	-t)
	mysql -uroot -e "use portal_conf;select * from portal_service_config where Fservice_type=\"$2\" limit 1\G"
	if [ $? != 0 ]; then 
		echo "servicetype error" 
	fi

	;;
esac

exit 0


