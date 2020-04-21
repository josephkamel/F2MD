set   autoscale                        # scale axes automatically
set style data points
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                          # set xtics automatically
set ytic auto                          # set ytics automatically
set title "Data Points"
set xlabel "Time (Min)"
set ylabel "Messages (Num)"
set grid 
set key right top
show grid 
funcT(x) = x / 60
#func3(x,y) = y<=0  ? 0 : 100*x / y
func3(x,y) = x

plot "AppV1Inst.dat" using (funcT($2)):(func3($4,$5)) title 'V1 False Positive Inst' ps 0.5 lc rgb "magenta" , \
 "AppV1Inst.dat" using (funcT($2)):(func3($6,$7)) title 'V1 True Positive Inst' ps 0.5 lc rgb "#CD853F", \
 "AppV2Inst.dat" using (funcT($2)):(func3($4,$5)) title 'V2 False Positive Inst'  ps 0.5 lc rgb "#4dbeee" , \
 "AppV2Inst.dat" using (funcT($2)):(func3($6,$7)) title 'V2 True Positive Inst' ps 0.5 lc rgb "#009000"  

#plot "AppV1.dat" using (funcT($2)):(func3($4,$5)) title 'V1 False Positive' with lines lw 3 lc rgb "magenta" , \
# "AppV1Inst.dat" using (funcT($2)):(func3($4,$5)) title 'V1 False Positive Inst' ps 0.5 lc rgb "magenta" , \
# "AppV1.dat" using (funcT($2)):(func3($6,$7)) title 'V1 True Positive' with lines lc rgb "yellow" lw 3 , \
# "AppV1Inst.dat" using (funcT($2)):(func3($6,$7)) title 'V1 True Positive Inst' ps 0.5 lc rgb "yellow", \
# "AppV2.dat" using (funcT($2)):(func3($4,$5)) title 'V2 False Positive' with lines lw 3 lc rgb "#4dbeee" , \
# "AppV2Inst.dat" using (funcT($2)):(func3($4,$5)) title 'V2 False Positive Inst'  ps 0.5 lc rgb "#4dbeee" , \
# "AppV2.dat" using (funcT($2)):(func3($6,$7)) title 'V2 True Positive' with lines lc rgb "#009000" lw 3 , \
# "AppV2Inst.dat" using (funcT($2)):(func3($6,$7)) title 'V2 True Positive Inst' ps 0.5 lc rgb "#009000"  


#plot "AppV1Inst.dat" using (funcT($2)):(func3($4,$5)) title 'V1 False Positive' ps 0.5 lc rgb "magenta" , \
# "AppV1Inst.dat" using (funcT($2)):(func3($6,$7)) title 'V1 True Positive ' ps 0.5 lc rgb "yellow", \
# "AppV2Inst.dat" using (funcT($2)):(func3($4,$5)) title 'V2 False Positive' ps 0.5 lc rgb "#4dbeee" , \
# "AppV2Inst.dat" using (funcT($2)):(func3($6,$7)) title 'V2 True Positive' ps 0.5 lc rgb "#009000"  


#plot "AppV1.dat" using (funcT($2)):(func3($4,$5)) title 'Cumulative False Positivey V1' with lines lw 3 lc rgb "magenta" , \
# "AppV1Inst.dat" using (funcT($2)):(func3($4,$5)) title 'Instantaneous False Positivey V1' ps 0.5 lc rgb "magenta" , \
# "AppV1.dat" using (funcT($2)):(func3($6,$7)) title 'Cumulative Attacker V1' with lines lc rgb "yellow" lw 3 , \
# "AppV1Inst.dat" using (funcT($2)):(func3($6,$7)) title 'Instantaneous Attacker V1' ps 0.5 lc rgb "yellow", \
# "AppV2.dat" using (funcT($2)):(func3($4,$5)) title 'Cumulative False Positivey V2' with lines lw 3 lc rgb "#4dbeee" , \
# "AppV2Inst.dat" using (funcT($2)):(func3($4,$5)) title 'Instantaneous False Positivey V2' ps 0.5 lc rgb "#4dbeee" , \
# "AppV2.dat" using (funcT($2)):(func3($6,$7)) title 'Cumulative Attacker V2' with lines lc rgb "#009000" lw 3 , \
# "AppV2Inst.dat" using (funcT($2)):(func3($6,$7)) title 'Instantaneous Attacker V2' ps 0.5 lc rgb "#009000"  

