set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
set title "On board Report Accuracy"
set xlabel "Time (Min)"
set ylabel "Falty Reports (%)"
set grid 
show grid 
funcAcc(x,y) = 100* x/(x+y)
funcT(x) = x/60

#TP 26
#FP 14
#FN 27-26
#TN 15-14

#TP 26
#FP 14
#FN 27-26
#TN 15-14

#27 -> 29
#26 -> 28
#15 -> 16
#14 -> 15

plot "AppV1.dat" using (funcT($2)):(funcAcc($6,$4)) title 'SOTA F-score' with lines lw 2 lc rgb "red" , \
"AppV1Inst.dat" using (funcT($2)):(funcAcc($6,$4)) title 'SOTA F-score Inst' ps 0.5 lc rgb "red" , \
"AppV2.dat" using (funcT($2)):(funcAcc($6,$4)) title 'SCA lib F-score' with lines lw 2 lc rgb "#4dbeee" , \
"AppV2Inst.dat" using (funcT($2)):(funcAcc($6,$4)) title 'SCA lib F-score Inst' ps 0.5 lc rgb "#4dbeee" 
