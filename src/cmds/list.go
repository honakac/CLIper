package cmds

import (
	"cliper/database"
	"fmt"
	"log"
	"strings"
	"time"

	"github.com/spf13/cobra"
)

func NewListCmd(db *database.Database) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "list",
		Short: "Print notes",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil {
				if err.Error() == "EOF" {
					fmt.Println("Empty!")
				} else {
					log.Fatal(err)
				}
			}

			for i, note := range db.NoteList {
				fmt.Printf("%08d: %s: %q (Tags: %s)\n",
					i, time.Unix(note.Timestamp, 0).Format(TIMESTAMP_FORMAT),
					note.Title, strings.Join(note.Tags, ", "),
				)
			}
		},
	}

	return
}
