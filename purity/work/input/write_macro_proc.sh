rm -f thedir/fit_mop.C
thenumber=`wc thedir/mop.input | awk '{print $1}' `
thepoints=$(echo "$thenumber/6" | bc )
# echo $thepoints
zero=0;
if [  $thepoints -gt $zero  ]
then
sed -i "s|TODRAW|$thepoints|" thedir/fit1_mop.input
cat thedir/fit1_mop.input > thedir/fit_mop.C
cat thedir/mop.input >> thedir/fit_mop.C
cat thedir/fit2_mop.input >> thedir/fit_mop.C
fi

