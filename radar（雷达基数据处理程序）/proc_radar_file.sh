source /home/radar_user/.bash_profile
#/home/radar_user/bin/ftp_datacloud.sh

#/home/radar_user/source/proc_radar_file 90 &
#/home/radar_user/source/proc_radar_file 91 &
#/home/radar_user/source/proc_radar_file 92 &
#/home/radar_user/source/proc_radar_file 93 &
#/home/radar_user/source/proc_radar_file 94 &
#/home/radar_user/source/proc_radar_file 95 &
#/home/radar_user/source/proc_radar_file 96 &
#/home/radar_user/source/proc_radar_file 97 &
#/home/radar_user/source/proc_radar_file 98 &
#/home/radar_user/source/proc_radar_file 99 &

#wait



find /home/radar_user/radardata/* -mmin +720 | xargs rm -fr
