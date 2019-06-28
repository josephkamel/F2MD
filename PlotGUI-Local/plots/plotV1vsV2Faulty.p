set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto      	                   # set xtics automatically
set ytic auto						   # set ytics automatically
set title "False Positives"
set xlabel "Time (Min)"
set ylabel "Messages (Num)"
set grid 
show grid 
set key right bottom
funcT(x) = x / 60
#func3(x,y) = y<=0  ? 0 : 100*x / y
func3(x,y) = x

stats "AppV1.dat" using (lastV1=func3($4,$5)) nooutput
stats "AppV2.dat" using (lastV2=func3($4,$5)) nooutput
stats "AppV1.dat" using (FNV1=($5-$4)) nooutput
stats "AppV2.dat" using (FNV2=($5-$4)) nooutput
set label sprintf("V1 = %3.10g-%3.10gmsg \nV2 = %3.10g-%3.10gmsg",lastV1, FNV1, lastV2, FNV2) at graph 0.02,0.95 tc rgb '#000000'

plot "AppV2.dat" using (funcT($2)):(func3($4,$5)) title 'V2 False Positive' with linespoints linestyle 2 lw 2 pi 30 ps 0.75 lc rgb "#4dbeee", \
"AppV1.dat" using (funcT($2)):(func3($4,$5)) title 'V1 False Positive' with lines lw 2 lc rgb "magenta", \

#"AppV1.dat" using (funcT($2)):($4) title 'SOTA Num' with linespoints lw 0.1 ps 0.1 lt rgb "magenta" axis x1y2, \
#"AppV2.dat" using (funcT($2)):($4) title 'SCA lib Num' with linespoints lw 0.1 ps 0.1 lt rgb "#4dbeee" axis x1y2, \
