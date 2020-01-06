set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto 			# set ytics automatically
set title "True Positives"
set xlabel "Time (Min)"
set ylabel "Messages (Num)"
set grid 
show grid 
set key right bottom
funcT(x) = x / 60
#func3(x,y) = y<=0  ? 0 : 100*x / y
func3(x,y) = x

stats "AppV1.dat" using (lastV1=func3($6,$7)) nooutput
stats "AppV2.dat" using (lastV2=func3($6,$7)) nooutput
stats "AppV1.dat" using (FNV1=($7-$6)) nooutput
stats "AppV2.dat" using (FNV2=($7-$6)) nooutput
set label sprintf("V1 = %3.10g-%3.10gmsg \nV2 = %3.10g-%3.10gmsg",lastV1, FNV1, lastV2, FNV2) at graph 0.02,0.95 tc rgb '#00bb00'

plot "AppV2.dat" using (funcT($2)):(func3($6,$7)) title 'V2 True Positive' with linespoints linestyle 2 lw 2 pi 30 ps 0.75 lc rgb "#009000",\
"AppV1.dat" using (funcT($2)):(func3($6,$7)) title 'V1 True Positive' with lines lw 2 lc rgb "#CD853F"

