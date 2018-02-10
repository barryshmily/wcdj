package main

import (
	"fmt"
	"flag"
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

	fmt.Println("iterator")
	iter := db.NewIterator(nil, nil)
	for iter.Next() {
		fmt.Printf("key[%#v][%s] value[%#v][%s]\n", iter.Key(), iter.Key(), iter.Value(), iter.Value())
	}
	iter.Release()
}
