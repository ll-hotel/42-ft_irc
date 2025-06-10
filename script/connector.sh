if [ $# -ne 3 ]; then
echo '<nickname> <time> <target>'
exit 1
fi

./script/join_script.sh $1 $2 | nc -C $3 8080
exit 0
