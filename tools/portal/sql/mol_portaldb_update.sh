#!/bin/bash
# mol_portaldb_update.sh for sandbox

INFO="-h10.137.133.144 -uportal -pportal portal_conf -e"
SERVICECODE="-APPSX17865"

mysql $INFO "update portal_service_config set Fchannel_list=concat(Fchannel_list,',mol') where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update Fchannel_list err!"
fi

mysql $INFO "update portal_service_config set Fip_list=concat(Fip_list,',10.153.130.94') where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update Fip_list err!"
fi

mysql $INFO "update portal_service_config set Fapp_server_type='cgi' where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update Fapp_server_type err!"
fi

mysql $INFO  "update portal_service_config set Ftodo_type='1',Fcgi_path_format='1' where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update Ftodo_type, Fcgi_path_format err!"
fi

mysql $INFO "update portal_service_config set Fmaster_pricing_ip='10.153.130.94',Fmaster_pricing_port='80' where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update  Fmaster_pricing_ip, Fmaster_pricing_port err!"
fi

mysql $INFO "update portal_service_config set Fslave_pricing_ip='10.153.130.94',Fslave_pricing_port='80' where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update  Fslave_pricing_ip, Fslave_pricing_port err!"
fi

mysql $INFO "update portal_service_config set Fapp_pricing_cgi='/item_manage/get_price.fcg' where Fservice_code=\"$SERVICECODE\"\G"
if [ $? != 0 ]; then
	echo "update Fapp_pricing_cgi err!"
fi

