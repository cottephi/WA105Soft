rm -f fit_mop.C 
thenumber=`wc mop.input | awk '{print $1}' `
thepoints=$(echo "$thenumber/6" | bc )  
# echo $thepoints
zero=0;
if [  $thepoints -gt $zero  ]
then  
sed -i "s|TODRAW|$thepoints|" fit1_mop.input
cat fit1_mop.input > fit_mop.C
cat mop.input >> fit_mop.C 
cat fit2_mop.input >> fit_mop.C 
fi
