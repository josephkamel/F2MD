set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
#set yrange [0:1] 
set yrange [-1:1]
#unset yrange 
set title "Matthews Correlation Coefficient (MCC)"
set xlabel "Time (Min)"
set ylabel "MCC (Abs)"
set grid 
show grid 
set key right bottom
funcAcc(x,y) = 100* x/(x+y)
funcT(x) = x/60

#TP 26
#FP 14
#FN 27-26
#TN 15-14

#sensitivity TPR=TP/(TP+FN)
TPR(x,y) = x/(x+y)

#precision PPV=TP/(TP+FP)
PPV(x,y) = x/(x+y)

#F1 score F1=2 TP/(2 TP+ FP+ FN)
F(x,y,z) = 2*x/(2*x+y+z)

#F1 score 
FB(x,y) = (2*(x*y))/(x+y)

# Info BM=TP/TP+FN + TN/TN+FP − 1
# Mark MK=TP/TP+FP + TN/TN+FN − 1
MK(a,b,c,d) = (a)/(a+d)+c/(c+b) - 1

MCC(TP,FN,TN,FP) = ((TP*TN)-(FP*FN))/sqrt((TP+FP)*(TP+FN)*(TN+FP)*(TN+FN))


stats "AppV1.dat" using (lastV1=MCC($6,($7-$6),($5-$4),$4)) nooutput
stats "AppV2.dat" using (lastV2=MCC($6,($7-$6),($5-$4),$4)) nooutput
set label sprintf("V1 = %3.5g \nV2 = %3.5g", lastV1, lastV2) at graph 0.02,0.1 tc rgb '#00bb00'

plot "AppV2.dat" using (funcT($2)):(MCC($6,($7-$6),($5-$4),$4)) title "V2 MCC" with linespoints linestyle 2 lw 2 pi 30 ps 0.75 lc rgb "blue",\
"AppV1.dat" using (funcT($2)):(MCC($6,($7-$6),($5-$4),$4)) title "V1 MCC" with lines lw 2 lc rgb "red"


