for i in {1..1000}; do
    ./build/SkipListTest
	if [ $? -ne 0 ]; then
        echo "Error detected in iteration $i. Stopping."
        exit 1
    fi
	echo i
done
