package cmds

import (
	"cliper/database"
	"log"
	"strings"
	"time"

	"github.com/spf13/cobra"
)

func NewAppendCmd(db *database.Database, opts *AppendOptions) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "append",
		Short: "Create new note",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil && err.Error() != "EOF" {
				log.Fatal(err)
			}

			db.Append(database.Note{
				Timestamp: time.Now().Unix(),
				Tags:      strings.Split(opts.Tags, ","),
				Title:     opts.Title,
				Content:   opts.Content,
			})

			if err := db.Write(); err != nil {
				log.Fatal(err)
			}
		},
	}

	cmd.Flags().StringVarP(&opts.Title, "title", "t", "", "Title")
	cmd.Flags().StringVarP(&opts.Tags, "tags", "g", "", "Tags (separable comma character)")
	cmd.Flags().StringVarP(&opts.Content, "content", "c", "", "Content")

	cmd.MarkFlagRequired("title")

	return
}
