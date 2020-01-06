set   autoscale                        # scale axes automatically
unset log                              # remove any log-scaling
unset label                            # remove any previous labels
set xtic auto                        # set xtics automatically
set ytics auto                        # set ytics automatically
set y2tics auto		# set ytics automatically
set title  "Vehicle Density" font ",18,bold"
set xlabel "Time (Min)"
set ylabel "Density (Active Cars)"
set grid 
show grid
set key right bottom

stats "AppV1.dat" using (lastV1=$3) nooutput
set label sprintf("d = %3.5gveh",lastV1) at graph 0.02,0.1 tc rgb '#00bb00'

plot "AppV1.dat" using (funcT($2)):($3) title 'Density' with lines lw 2 lc rgb "#009000" axes x1y1
