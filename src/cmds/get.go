package cmds

import (
	"cliper/database"
	"fmt"
	"log"
	"strconv"
	"strings"
	"time"

	"github.com/spf13/cobra"
)

func NewGetCmd(db *database.Database, opts *GetOptions) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "get",
		Short: "Get note by index",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil && err.Error() != "EOF" {
				log.Fatal(err)
			}

			if opts.Index >= len(db.NoteList) || opts.Index < 0 {
				log.Fatal("Invalid index " + strconv.Itoa(opts.Index))
			}

			note := db.NoteList[opts.Index]
			noteTimestamp := time.Unix(int64(note.Timestamp), 0).Format(TIMESTAMP_FORMAT)
			noteTags := strings.Join(note.Tags, ", ")

			fmt.Printf("%s %q (Tags: %s):\n%s\n", noteTimestamp, note.Title, noteTags, note.Content)

			if err := db.Write(); err != nil {
				log.Fatal(err)
			}
		},
	}

	cmd.Flags().IntVarP(&opts.Index, "index", "i", 0, "Index note")

	cmd.MarkFlagRequired("index")

	return
}
