#!/bin/bash

path=$(realpath $0)
dirpth=$(dirname $path)

#reading config values
source CONF.config

#installing hardware
echo "Installing hardware . . . "
cd hardware-setup/
touch stm_tmp.sh
sed "s/xxxx/$IFNAME/" stm.sh > stm_tmp.sh
sudo cp stm_tmp.sh /usr/bin/stm.sh
sudo chmod +x /usr/bin/stm.sh
sudo cp stm_eth_startup.service /etc/systemd/system/stm_eth_startup.service
sudo chmod 644 /etc/systemd/system/stm_eth_startup.service
sudo systemctl start stm_eth_startup
sudo systemctl enable stm_eth_startup
sudo systemctl daemon-reload
rm stm_tmp.sh
cd ..


#configuring sercos module
cd source
fname='sercos.comp.template'
fname_t='sercos.comp'

cp $fname $fname_t

# global parameters

sed -i "s/#VERBOSE#/$VERBOSE/g" $fname_t
sed -i "s+#SYNC_FILE_NAME#+$SYNC_FILE_PATH+g" $fname_t

sed -i "s/#MAX_RT_CYCLEN#/$MAX_RT_CYCLEN/g" $fname_t
# sed -i "s/#EXPECTED_READ_POSITION#/$EXPECTED_READ_POSITION/g" $fname_t


# ethenet
sed -i "s/#IFNAME#/$IFNAME/g" $fname_t
sed -i "s/#ETH_BUF_SIZE#/$ETH_BUF_SIZE/g" $fname_t

sed -i "s/#HW_MAC_0#/$HW_MAC_0/g" $fname_t
sed -i "s/#HW_MAC_1#/$HW_MAC_1/g" $fname_t
sed -i "s/#HW_MAC_2#/$HW_MAC_2/g" $fname_t
sed -i "s/#HW_MAC_3#/$HW_MAC_3/g" $fname_t
sed -i "s/#HW_MAC_4#/$HW_MAC_4/g" $fname_t
sed -i "s/#HW_MAC_5#/$HW_MAC_5/g" $fname_t

sed -i "s/#EMC_MAC_0#/$EMC_MAC_0/g" $fname_t
sed -i "s/#EMC_MAC_1#/$EMC_MAC_1/g" $fname_t
sed -i "s/#EMC_MAC_2#/$EMC_MAC_2/g" $fname_t
sed -i "s/#EMC_MAC_3#/$EMC_MAC_3/g" $fname_t
sed -i "s/#EMC_MAC_4#/$EMC_MAC_4/g" $fname_t
sed -i "s/#EMC_MAC_5#/$EMC_MAC_5/g" $fname_t

# drives config
sed -i "s/#SERC_MAX_MESSAGE_SIZE#/$SERC_MAX_MESSAGE_SIZE/g" $fname_t
sed -i "s/#DRIVE_NUM#/$DRIVE_NUM/g" $fname_t
for (( i=0; i < $DRIVE_NUM; i++ ))
do
	declare -n addr="DRIVE${i}_ADDR"
	declare -n in_n="DRIVE${i}_IDN_IN_NUM"
	declare -n out_n="DRIVE${i}_IDN_OUT_NUM"
	echo "found drive $addr with $in_n in tg and $out_n out tg."
	sed -i "s/#drives#/drives[${i}].addr =${addr};\n	#drives#/g" $fname_t
	sed -i "s/#input_telegrams#/pin out signed drive-${i}-in-#[${in_n}];\n#input_telegrams#/g" $fname_t
	sed -i "s/#output_telegrams#/pin in signed drive-${i}-out-#[${out_n}];\n#output_telegrams#/g" $fname_t

	# in idns
	sed -i "s/#drives#/drives[${i}].idn_in_number =${in_n};\n	#drives#/g" $fname_t
	for (( j=0; j < $in_n; j++ ))
	do
		declare -n size="DRIVE${i}_IDN_IN${j}_SIZE"
		sed -i "s/#drives#/drives[${i}].idn_in_size[${j}]=${size};\n	#drives#/g" $fname_t
		sed -i "s/#in_pins#/drive_${i}_in(${j}) = drives[${i}].idn_in[${j}];\n	#in_pins#/g" $fname_t

	done
	
	# out idns
	sed -i "s/#drives#/drives[${i}].idn_out_number =${out_n};\n	#drives#/g" $fname_t

	for (( j=0; j < $out_n; j++ ))
	do
		declare -n size="DRIVE${i}_IDN_OUT${j}_SIZE"
		sed -i "s/#drives#/drives[${i}].idn_out_size[${j}]=${size};\n	#drives#/g" $fname_t
		sed -i "s/#out_pins#/drives[${i}].idn_out[${j}] = drive_${i}_out(${j});\n	#out_pins#/g" $fname_t
	done
done
sed -i "s/#drives#/\n/g" $fname_t
sed -i "s/#output_telegrams#/\n/g" $fname_t
sed -i "s/#input_telegrams#/\n/g" $fname_t
sed -i "s/#in_pins#/\n/g" $fname_t
sed -i "s/#out_pins#/\n/g" $fname_t


sudo $LINUXCNC/bin/halcompile --install $fname_t
#rm $fname_t


#adding rights to use ethernet hardware
sudo chown root:root $LINUXCNC/rtlib/sercos.so
sudo chmod u+s $LINUXCNC/rtlib/sercos.so
