rm -f tbin.root
echo '----->processing input file'
purity.exe -f $1  
echo '----->fitting all histograms '
thefile=`echo $1 | awk -F'.' '{print $1}'`
#now=$(date +"%T-%d-%m-%Y")  
now=$(date +"%d%m%y_%H%M%S")
echo '----->creating directory and script'
mkdir purity_"$thefile"_"$now"  
cd  purity_"$thefile"_"$now"  
touch fitresult_"$now".listing
echo $now > fitresult_"$now".listing
rm -f fit.listing 
ln -s fitresult_"$now".listing fit.listing
cp ../input/fitbin.C .
cp ../input/write_macro.sh .
cp ../input/fit1_mop.input .
cp ../input/fit2_mop.input .
rm -f tbin.root
mv ../purity_$thefile.* . 
ln -s purity_$1 tbin.root 
root -q -b fitbin.C  
source  write_macro.sh 

if [ -f fit_mop.C ] 
then
root -l  fit_mop.C 
mv purity.pdf purity_$now.pdf
fi 
