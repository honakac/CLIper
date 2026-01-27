package main

import (
	"cliper/database"
	"fmt"
	"os"
)

func main() {
	db := database.Init()
	defer db.Close()

	rootCmd := NewRootCmd(&db)

	if err := rootCmd.Execute(); err != nil {
		fmt.Fprint(os.Stderr, err)
		os.Exit(1)
	}
}
