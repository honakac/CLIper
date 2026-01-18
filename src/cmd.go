package main

import (
	"fmt"
	"os"
	"strings"
	"time"

	"cliper/database"

	"github.com/spf13/cobra"
)

type AppendOptions struct {
	Title   string
	Tags    string
	Content string
}
type RemoveOptions struct {
	Index int
}
type GetOptions struct {
	Index int
}

const VERSION string = "1.2.0"
const TIMESTAMP_FORMAT string = "2006-01-02 15:04:05"

var VersionCmd = &cobra.Command{
	Use:   "version",
	Short: "Show version",
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("CLIper v" + VERSION)
	},
}

func NewAppendCmd(db *database.Database, opts *AppendOptions) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "append",
		Short: "Create new note",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil && err.Error() != "EOF" {
				panic(err)
			}

			db.Append(database.Note{
				Timestamp: time.Now().Unix(),
				Tags:      strings.Split(opts.Tags, ","),
				Title:     opts.Title,
				Content:   opts.Content,
			})

			if err := db.Write(); err != nil {
				panic(err)
			}
		},
	}

	cmd.Flags().StringVarP(&opts.Title, "title", "t", "", "Title")
	cmd.Flags().StringVarP(&opts.Tags, "tags", "g", "", "Tags (separable comma character)")
	cmd.Flags().StringVarP(&opts.Content, "content", "c", "", "Content")

	cmd.MarkFlagRequired("title")

	return
}

func NewListCmd(db *database.Database) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "list",
		Short: "Print notes",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil {
				if err.Error() == "EOF" {
					fmt.Println("Empty!")
				} else {
					panic(err)
				}
			}

			for i, note := range db.NoteList {
				fmt.Printf("%08d: %s: %q (Tags: %q)\n",
					i, time.Unix(note.Timestamp, 0).Format(TIMESTAMP_FORMAT),
					note.Title, strings.Join(note.Tags, ", "),
				)
			}
		},
	}

	return
}

func NewRemoveCmd(db *database.Database, opts *RemoveOptions) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "remove",
		Short: "Remove note by index",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil && err.Error() != "EOF" {
				panic(err)
			}

			if opts.Index >= len(db.NoteList) || opts.Index < 0 {
				fmt.Fprintln(os.Stderr, "Invalid index!")
				os.Exit(2)
			}
			db.Remove(opts.Index)

			if err := db.Write(); err != nil {
				panic(err)
			}
		},
	}

	cmd.Flags().IntVarP(&opts.Index, "index", "i", 0, "Index note")

	cmd.MarkFlagRequired("index")

	return
}

func NewGetCmd(db *database.Database, opts *GetOptions) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "get",
		Short: "Get note by index",
		Run: func(cmd *cobra.Command, args []string) {
			if err := db.Read(); err != nil && err.Error() != "EOF" {
				panic(err)
			}

			if opts.Index >= len(db.NoteList) || opts.Index < 0 {
				fmt.Fprintln(os.Stderr, "Invalid index!")
				os.Exit(2)
			}

			note := db.NoteList[opts.Index]
			noteTimestamp := time.Unix(int64(note.Timestamp), 0).Format(TIMESTAMP_FORMAT)
			noteTags := strings.Join(note.Tags, ", ")

			fmt.Printf("%s %q (Tags: %q):\n%s\n", noteTimestamp, note.Title, noteTags, note.Content)

			if err := db.Write(); err != nil {
				panic(err)
			}
		},
	}

	cmd.Flags().IntVarP(&opts.Index, "index", "i", 0, "Index note")

	cmd.MarkFlagRequired("index")

	return
}

func NewRootCmd(db *database.Database) (cmd *cobra.Command) {
	cmd = &cobra.Command{
		Use:   "cliper",
		Short: "CLIper - Lightweight CLI tool for managing notes in the terminal",
	}

	var appendOptions AppendOptions
	var removeOptions RemoveOptions
	var getOptions GetOptions

	cmd.AddCommand(
		VersionCmd,
		NewAppendCmd(db, &appendOptions),
		NewListCmd(db),
		NewRemoveCmd(db, &removeOptions),
		NewGetCmd(db, &getOptions),
	)

	return
}
