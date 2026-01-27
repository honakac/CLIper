package cmds

import (
	"cliper/database"
	"log"
	"strconv"

	"github.com/spf13/cobra"
)

func NewRemoveCmd(db *database.Database, opts *RemoveOptions) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "remove",
		Short: "Remove note by index",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil && err.Error() != "EOF" {
				log.Fatal(err)
			}

			if opts.Index >= len(db.NoteList) || opts.Index < 0 {
				log.Fatal("Invalid index " + strconv.Itoa(opts.Index))
			}
			db.Remove(opts.Index)

			if err := db.Write(); err != nil {
				log.Fatal(err)
			}
		},
	}

	cmd.Flags().IntVarP(&opts.Index, "index", "i", 0, "Index note")

	cmd.MarkFlagRequired("index")

	return
}
