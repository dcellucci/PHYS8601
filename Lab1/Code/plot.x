unset key
set term png
set output 'out.png'
set title 'Graph of Sample Sets, Their Mean and Standard Error'

f(x) = mean_y
fit f(x) 'out_pi.out' u 1:2 via mean_y

stddev_y = sqrt(FIT_WSSR / (FIT_NDF+1))/10

plot mean_y-stddev_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd", \
mean_y+stddev_y with filledcurves y1=mean_y lt 1 lc rgb "#bbbbdd", \
mean_y w l lt 3, 'out_pi.out' u 1:2 w p pt 7 lt 1
