make
for file in "./OS_PJ1_Test"/*; do
    echo "Processing $file..."
    tmp=${file%.txt}
    fname=${tmp##*/}
    outfile=./output/${fname}_stdout.txt
    msgfile=./output/${fname}_dmesg.txt
    sudo dmesg -c
    ./main.o < $file > $outfile
    dmesg | grep Project1 > $msgfile
done

echo "Completed!"
