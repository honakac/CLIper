package main

import (
	"fmt"
	"os"
)

func main() {
	RootCmd.AddCommand(VersionCmd)
	
	if err := RootCmd.Execute(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
