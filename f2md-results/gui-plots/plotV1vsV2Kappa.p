set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
#set yrange [0:1] 
set yrange [-1:1]
#unset yrange 
set title "Cohen's kappa (K)"
set xlabel "Time (Min)"
set ylabel "K (Abs)"
set grid 
show grid 
set key right bottom
funcAcc(x,y) = 100* x/(x+y)
funcT(x) = x/60

#TP 26
#FP 14
#FN 27-26
#TN 15-14

K(TP,FN,TN,FP)=(((TP+TN)/(TP+FP+FN+TN))-(((TP+FP)*(TP+TN)+(TN+FP)*(TN+FN))/((TN+TP+FP+FN)**2)))/(1-(((TP+FP)*(TP+TN)+(TN+FP)*(TN+FN))/((TN+TP+FP+FN)**2)))


stats "AppV1.dat" using (lastV1=K($6,($7-$6),($5-$4),$4)) nooutput
stats "AppV2.dat" using (lastV2=K($6,($7-$6),($5-$4),$4)) nooutput
set label sprintf("V1 = %3.5g \nV2 = %3.5g", lastV1, lastV2) at graph 0.02,0.1 tc rgb '#00bb00'

plot "AppV2.dat" using (funcT($2)):(K($6,($7-$6),($5-$4),$4)) title "V2 K1" with linespoints linestyle 2 lw 2 pi 30 ps 0.75 lc rgb "blue",\
"AppV1.dat" using (funcT($2)):(K($6,($7-$6),($5-$4),$4)) title "V1 K2" with lines lw 2 lc rgb "red"




