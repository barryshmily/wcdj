package main

import (
	"flag"
	"fmt"
	"github.com/syndtr/goleveldb/leveldb"
)

func main() {

	p := flag.String("p", "/root/db0/ledgersData/stateLeveldb", "path")
	flag.Parse()
	fmt.Println("path:", *p)

	db, err := leveldb.OpenFile(*p, nil)
	if err != nil {
		panic(err)
	}
	defer db.Close()


	/*
		data_a, _ := db.Get([]byte("mychannel\x00mycc\x00a"), nil)
		fmt.Printf("mychannelmycca[%s]\n", data_a)
		data_b, _ := db.Get([]byte("mychannel\x00mycc\x00b"), nil)
		fmt.Printf("mychannelmyccb[%s]\n", data_b)

		err = db.Put([]byte("mychannel\x00mycc\x00a"), []byte("\x01\x02\x001000"), nil)
		data_a, _ = db.Get([]byte("mychannel\x00mycc\x00a"), nil)
		fmt.Printf("new mychannelmycca[%s]\n", data_a)

		err = db.Put([]byte("mychannel\x00mycc\x00b"), []byte("\x01\x02\x002000"), nil)
		data_b, _ = db.Get([]byte("mychannel\x00mycc\x00b"), nil)
		fmt.Printf("new mychannelmyccb[%s]\n", data_b)
	*/

	//ckey := "mychannel\x00\x00"
	key := "mychannel\x00fabcar\x00CAR100"

	value, _ := db.Get([]byte(key), nil)
	fmt.Printf("key[%s] value[%s]\n", key, value)

	err = db.Put([]byte(key),
		[]byte("\x01\x03\x00{\"make\":\"BMW\",\"model\":\"320\",\"colour\":\"Blue\",\"owner\":\"Gerry\",\"value\":\"200\"}"), nil)
	value, _ = db.Get([]byte(key), nil)
	fmt.Printf("new key[%s] value[%s]\n", key, value)

	/*
		cvalue, _ := db.Get([]byte(ckey), nil)
		fmt.Printf("ckey[%s] cvalue[%s]\n", ckey, cvalue)

		err = db.Put([]byte(ckey),
				[]byte("\x01\x03\x00"), nil)
		cvalue, _ = db.Get([]byte(ckey), nil)

		fmt.Printf("new ckey[%s] cvalue[%s]\n", ckey, cvalue)
	*/

}
