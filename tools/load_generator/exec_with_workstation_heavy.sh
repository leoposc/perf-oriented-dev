killall loadgen &> /dev/null


for i in {1..20}; do
    ../build/loadgen mc3 workstation/sys_load_profile_workstation_excerpt.txt &> /dev/null &
done

#time -p nice -n 100 $1
# wait
nice -n 19 ./run.sh
killall loadgen &> /dev/null
