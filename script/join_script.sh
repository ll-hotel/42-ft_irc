if [ $# -ne 2 ]; then
echo '<nickname> <time>'
exit 1
fi

echo PASS bocal
echo NICK $1
echo USER $1 '* * *'
sleep 5
for i in $(seq 0 $2); do
chan_id="_"$(shuf -i 10-100 -n 1)
echo 'MOTD'
echo 'JOIN ' '#chan'$chan_id
echo 'NAMES ' '#chan'$chan_id
echo 'MODE +i'
echo 'MODE +k test'
echo -n 'PRiVMSG '
echo '#chan'$chan_id ':'$(shuf -i 1000-100000 -n 4 | tr '\n' ' ')
echo 'PART ' '#chan'$chan_id
echo 'who *'
echo -e '\a'
done
sleep 120
echo "quit"

exit 0