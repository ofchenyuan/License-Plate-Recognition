#!/bin/bash

#作用：规格化pos文件内容

# $1-->文件夹位置
# $2-->输出文件名称
# $3-->每张图片正样本目标个数
# $4-->图片坐标x
# $5-->图片坐标y
# $6-->图片宽度
# $7-->图片高度

location=$1
output_file_name=$2
target_num=$3
x=$4
y=$5
weigh=$6
height=$7

cd $1
cd ..
current_folder=`pwd`

if [ -f "$output_file_name" ];
then
	rm $output_file_name
	echo "remove old file  $output_file_name succesed!"
fi

touch $output_file_name
echo "recreat $output_file_name succesed!"

file_num=0
for line in $(ls $location)
do	
	echo "$location$line $target_num $x $y $weigh $height" >> $current_folder/$output_file_name
	echo "$location$line $target_num $x $y $weigh $height 已写入 $current_folder/$output_file_name !"
	let file_num=$file_num+1
done

echo "共写入$file_num条记录！"
