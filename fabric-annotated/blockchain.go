package main

import (
	"crypto/sha256"
	"fmt"
	"strings"
	"time"
)

type block struct {
	id           int
	data         string
	hash         string
	previousHash string
	timestamp    time.Time
}

func (b *block) sha256() string {
	h := sha256.New()
	h.Write([]byte(fmt.Sprintf("%d%s%s%s", b.id, b.data, b.previousHash, b.timestamp)))
	b.hash = fmt.Sprintf("%x", h.Sum(nil))
	return b.hash
}

type blockchain struct {
	blocks       []block
	previousHash string
}

func newBlockChain(data string) blockchain {
	b := block{
		0,
		data,
		"",
		strings.Repeat("0", 64),
		time.Now(),
	}
	b.hash = b.sha256()
	return blockchain{
		blocks:       []block{b},
		previousHash: b.hash,
	}
}

func (bc *blockchain) newBlock(data string) {
	b := block{
		len(bc.blocks),
		data,
		"",
		bc.previousHash,
		time.Now(),
	}
	b.hash = b.sha256()
	bc.blocks = append(bc.blocks, b)
	bc.previousHash = b.hash
}

func (b *blockchain) printBlocks() {
	fmt.Println(b)
	for i, b := range b.blocks {
		fmt.Printf("block %d: %+v\n", i, b)
	}
}

func main() {
	bc := newBlockChain("genesis block")
	bc.newBlock("first block")
	bc.newBlock("second block")
	bc.newBlock("third block")
	bc.printBlocks()
}