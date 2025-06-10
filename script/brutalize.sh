if [ $# -ne 3 ]; then
echo '<client number> <time> <target>'
exit 1
fi

for i in $(seq 1 $1); do
./script/connector.sh 'script_'$i $2 $3 &
done

wait