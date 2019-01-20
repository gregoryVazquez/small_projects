#!/bin/bash

# note: script should be crontabed
# create_tmp_file: creates tmp file from the first 15 chars of
# hashed random number "returns" filename into the arg provided
# like: create_tmp_file [FILE] --NOTE-- no '$' in front of arg
# mktemp also great idea to use instead
create_tmp_file() {
	local _TMP_FILE=$1
	local _NAME="/tmp/$(echo $RANDOM | shasum | head -c15).$$"
	eval "$_TMP_FILE='$_NAME'"
}


# clears and logs files
# arg order tmp file, tmpfile, log file, pid file
kill_clear_and_log() {
	local _TMP_FILE_ONE=$1
	local _TMP_FILE_TWO=$2
	local _LOGFILE=$3
	local _PID_FILE=$4 
	if [ -s $_TMP_FILE_TWO ];then
		cat $_TMP_FILE_TWO | 
			tee -a $_LOGFILE |       			# your special logfile, in this case logfile.log 
				cut -d' ' -f1-4 | logger		# general system log
		cat $_PID_FILE |
			while read _KILL_PID;do
				kill -s SIGKILL $_KILL_PID 2>/dev/null
			done
		shred -zu $_PID_FILE
	fi
	shred -zu $_TMP_FILE_ONE $_TMP_FILE_TWO
}

# note: functions above could be placed in a .cnfg file and used 
# in a " . $HOME/yourFunctions.cnfg " fashion instead

create_tmp_file TMP_FILE_ONE
create_tmp_file TMP_FILE_TWO
LOG_FILE="./logfile.log"
create_tmp_file PID_TO_KILL

trap "kill_clear_and_log $TMP_FILE_ONE $TMP_FILE_TWO $LOG_FILE $PID_TO_KILL" \
	EXIT
ps aux > $TMP_FILE_ONE
# NOTE msfconsole is the proc picked to monitor and kill, replace w/ whatever proc needs to be monitored/killed
awk -v DATE="$(date)" '/msfconsole/ { printf "[TIMBER] User: %s %d %s %s\n", $1, $2, $11, $DATE }' \
	$TMP_FILE_ONE > $TMP_FILE_TWO
	
