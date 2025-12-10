#
# Regular cron jobs for the liballocs package.
#
0 4	* * *	root	[ -x /usr/bin/liballocs_maintenance ] && /usr/bin/liballocs_maintenance
