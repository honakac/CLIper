package main

import (
	"fmt"

	"github.com/spf13/cobra"
)

var RootCmd = &cobra.Command{
	Use: "cliper",
	Short: "CLIper - Lightweight CLI tool for managing notes in the terminal",
}

var VersionCmd = &cobra.Command{
	Use: "version",
	Short: "Show version",
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("CLIper 1.2.0")
	},
}