
#################################
## set environment              #
#################################
source /usr/mdss_user/.bash_profile

#################################
## RAWSH                        #
#################################
cd      /usr/mdss_user/mdss/read_tianqi_wis



read_rgwst_mdss 20160405  010000  010500 0100
read_rgwst_mdss 20160405  020000  020500 0200
read_rgwst_mdss 20160405  030000  030500 0300
read_rgwst_mdss 20160405  040000  040500 0400
read_rgwst_mdss 20160405  050000  050500 0500
read_rgwst_mdss 20160405  060000  060500 0600
read_rgwst_mdss 20160405  070000  070500 0700
read_rgwst_mdss 20160405  080000  080500 0800
read_rgwst_mdss 20160405  090000  090500 0900
read_rgwst_mdss 20160405  100000  100500 1000





 #Get rgwsp
      FTP_NAME_FILE="/tmp/ftp.rgwst.file.240"
      echo "user pmsc_yjs_provider  pmsc_yjs_provider123" > $FTP_NAME_FILE
      echo "bin" >>  $FTP_NAME_FILE
      echo "prom" >>  $FTP_NAME_FILE
      echo "cd /mnt/data_nfs/cma/pmsc/observe/rgwst_wis/txt" >> $FTP_NAME_FILE
      echo "lcd /usr/mdss_user/data/rgwst" >> $FTP_NAME_FILE
      echo "mput rgwst_20160405*" >> $FTP_NAME_FILE
     #  echo "mput rgwst_20151209*" >> $FTP_NAME_FILE

 

       


      ftp -nv 10.30.16.240 < $FTP_NAME_FILE


# rm -f  /usr/mdss_user/data/rgwst/rgwst_20160405*
