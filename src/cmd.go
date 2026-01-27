package main

import (
	"fmt"

	"cliper/cmds"
	"cliper/database"

	"github.com/spf13/cobra"
)

const VERSION string = "1.2.0"

var VersionCmd = &cobra.Command{
	Use:   "version",
	Short: "Show version",
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("CLIper v" + VERSION)
	},
}

func NewRootCmd(db *database.Database) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "cliper",
		Short: "CLIper - Lightweight CLI tool for managing notes in the terminal",
	}

	var appendOptions cmds.AppendOptions
	var removeOptions cmds.RemoveOptions
	var getOptions cmds.GetOptions

	cmd.AddCommand(
		VersionCmd,
		cmds.NewAppendCmd(db, &appendOptions),
		cmds.NewListCmd(db),
		cmds.NewRemoveCmd(db, &removeOptions),
		cmds.NewGetCmd(db, &getOptions),
	)

	return
}
