#!/bin/bash


############## cmd_config #################
					  #
# $1-->训练文件夹的位置			  #
# $2-->正样本图片宽度			  #
# $3-->正样本图片高度			  #
# $4-->负样本图片宽度			  #
# $5-->负样本图片高度			  #
					  #
cd $1					  #
train_dir=`pwd`				  #
resize_pos_width=$2			  #
resize_pos_height=$3			  #
resize_neg_width=$4			  #
resize_neg_height=$5			  #
					  #
###########################################

############ train_config #################
					  #
type="HAAR"                               #训练特征
index=0					  #规格化图片起始下标
target_num=1                              #每张正样本图片目标个数
x=0					  #图片起始x坐标
y=0					  #图片起始y坐标
precalcValBufSize=2048			  #
precalcIdxBufSize=2048			  #
numStages=15				  #	
					  #
##########################################


echo "#########################################   SIMPLE INFO    ############################################"
echo "训练文夹夹位置是：$train_dir"

if [ ! -f "log.txt" ];
then
	touch log.txt
fi

echo "################    `date`    #################" >> $train_dir/log.txt

if [ -d "pos_train" ];
then
	rm -rf pos_train
	mkdir pos_train
	./change_img $train_dir/pos/ $train_dir/pos_train/ $resize_pos_width $resize_pos_height $index >> $train_dir/log.txt
	echo "change_img for pos 执行成功，详细信息查看$train_dir/log.txt文件"

else
	mkdir pos_train
        ./change_img $train_dir/pos/ $train_dir/pos_train/ $resize_pos_width $resize_pos_height $index >> $train_dir/log.txt
	echo "change_img for pos 执行成功，详细信息查看$train_dir/log.txt文件"
fi

if [ -d "neg_train" ];
then
        rm -rf neg_train
        mkdir neg_train
        ./change_img $train_dir/neg/ $train_dir/neg_train/ $resize_neg_width $resize_neg_height $index >> $train_dir/log.txt
	echo "change_img for neg 执行成功，详细信息查看$train_dir/log.txt文件"
else
        mkdir neg_train
        ./change_img $train_dir/neg/ $train_dir/neg_train/ $resize_neg_width $resize_neg_height $index >> $train_dir/log.txt
	echo "change_img for neg 执行成功，详细信息查看$train_dir/log.txt文件"
fi

sleep 15s

./normalized_pos.sh $train_dir/pos_train/ pos.dat $target_num $x $y $resize_pos_width $resize_pos_height >> $train_dir/log.txt
echo "标准pos_train描述文件pos.dat生成成功，详细信息查看$train_dir/log.txt文件"
./normalized_pos.sh $train_dir/neg_train/ neg.txt >> $train_dir/log.txt
echo "标准neg_train描述文件neg.txt生成成功，详细信息查看$train_dir/log.txt文件"

pos_real_num=`cat pos.dat | wc -l`

let pos_vec_num=(999*$pos_real_num)/1000
let pos_vec_num=`echo $pos_vec_num | awk -F"." '{print $1}'`

let pos_train_num=(9*$pos_vec_num)/10
let pos_train_num=`echo $pos_vec_num | awk -F"." '{print $1}'`
#pos_train_num=800

neg_num=`cat neg.txt | wc -l`
let neg_train_num=(49*$neg_num)/50
let neg_train_num=`echo $neg_train_num | awk -F"." '{print $1}'`
#neg_train_num=2400 

echo "正样本个数：$pos_real_num"
echo "被描述正样本个数：$pos_vec_num"
echo "每级参加训练的正样本个数：$pos_train_num"
echo "负样本个数：$neg_num"
echo "每级参加训练的负样本个数：$neg_train_num"

echo "#########################################   SIMPLE INFO END   #########################################"
echo
echo "#########################################   opencv_createsamples INFO    ##############################"

if [ -f "pos.vec" ];
then
        rm -rf pos.vec
	./opencv_createsamples -vec pos.vec -info pos.dat -bg neg.txt -num $pos_vec_num -w $resize_pos_width -h $resize_pos_height
else
	./opencv_createsamples -vec pos.vec -info pos.dat -bg neg.txt -num $pos_vec_num -w $resize_pos_width -h $resize_pos_height        
fi

echo "#########################################   opencv_createsamples INFO END   ###########################"
echo

if [ -d "data" ];
then
        rm -rf data
        mkdir data
else
        mkdir data
fi

echo "#########################################   opencv_traincascade INFO    ##############################"
./opencv_traincascade -data data -vec pos.vec -bg neg.txt -numPos $pos_train_num -numNeg $neg_train_num -numStages $numStages -w $resize_pos_width -h $resize_pos_height -minHitRate 0.9999 -maxFalseAlarmRate 0.5 -featureType $type -mode ALL -precalcValBufSize $precalcValBufSize -precalcIdxBufSize $precalcIdxBufSize









