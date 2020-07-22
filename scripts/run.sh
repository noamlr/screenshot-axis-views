#!/bin/bash
root_dir=/home/noa/Desktop/temp/test/3D-views-script
executable_path=/home/noa/Documents/UFRGS/PROJECTS/CIDIA19/covid-gui-old/examples/ScreenshotUtilities/build/

array_patient=( "exame1" "exame2" "exame3" "exame4" "exame5" "exame6" "exame7" "exame8" "exame9" "exame10" 
	"exame11" "exame12" "exame13" "exame14" "exame15" "exame16" "exame17" "exame18" "exame19" "exame20" "exame21" )
array_view=( "axis1" "axis2" "axis3 axis4")

array_image_output2d=( 
	"/home/noa/Desktop/temp/test/3D-views-script/exame1/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame2/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame3/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame4/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame5/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame6/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame7/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame8/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame9/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame10/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame11/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame12/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame13/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame14/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame15/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame16/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame17/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame18/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame19/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame20/"
	"/home/noa/Desktop/temp/test/3D-views-script/exame21/"
)

array_image_input3d=( 
	"/home/noa/Downloads/Lung crop and mask/exame1/OUL0CZ5E/GRUPKMLN/GRUPKMLN_crop_by_mask.nii.gz"
	"/home/noa/Downloads/Lung crop and mask/exame2/SXDWNPE2/NMKW5E5D/NMKW5E5D_crop_by_mask.nii.gz"
	"/home/noa/Downloads/Lung crop and mask/exame3/ES2GJ21P/2OPQL5YQ/2OPQL5YQ_crop_by_mask.nii.gz"
	"/home/noa/Downloads/Lung crop and mask/exame4/VQ2KRLB2/K55KYYBG/K55KYYBG_crop_by_mask.nii.gz"
	"/home/noa/Downloads/Lung crop and mask/exame5/FQMAC1DT/D2KSTXMT/D2KSTXMT_crop_by_mask.nii.gz"
	"/home/noa/Downloads/Lung crop and mask/exame6/RJY5FFRD/LUTTOPM0/LUTTOPM0_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame7/FAMDN1BX/C0LGTVKT/C0LGTVKT_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame8/FACCYNVK/PW4YCRXA/PW4YCRXA_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame9/3XSGMZP0/KLBWYWNU/KLBWYWNU_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame10/KRK2JGZK/YMTSVLAD/YMTSVLAD_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame11/QM2OECZS/UFLR0JKB/UFLR0JKB_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame12/5OP45XQO/A3KB4TPB/A3KB4TPB_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame13/AKUN305M/O0ZQCS5H/O0ZQCS5H_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame14/IHF43LRS/GCCJIB34/GCCJIB34_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame15/R0Q1RRHU/RJESORWI/RJESORWI_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame16/32VP12L3/PVFJ4OYU/PVFJ4OYU_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame17/L5LQFSIF/RI0I3MCH/RI0I3MCH_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame18/QURAI2ZN/Q13X3X52/Q13X3X52_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame19/L1VEY5ZJ/ITOCNPCM/ITOCNPCM_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame20/VCEQWILJ/VJUJY5DL/VJUJY5DL_crop_by_mask.nii"
	"/home/noa/Downloads/Lung crop and mask/exame21/TC_COV_IA_001/TCCOVIA001/TCCOVIA001_crop_by_mask.nii.gz"
	)

echo $root_dir;
cd $root_dir;
rm -r $root_dir/*;
for i in "${array_patient[@]}"
do
	# do whatever on $i
	mkdir $i;
	cd $i;
	for j in "${array_view[@]}"
	do
		mkdir $j;
	done 
	cd ..
done

cd $executable_path;
# ls;
len=${#array_image_input3d[@]}
for (( ii=0; ii<$len; ii++ ))
do
	 ./screenshotUtilities -tf /home/noa/Downloads/TF/tf19.xml -i "${array_image_input3d[$ii]}" -o "${array_image_output2d[$ii]}"
done
