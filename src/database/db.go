package database

import (
	"encoding/gob"
	"os"
	"slices"
)

type Note struct {
	Timestamp int64
	Tags      []string
	Title     string
	Content   string
}

type Database struct {
	File     *os.File
	Path     string
	NoteList []Note
}

func (db *Database) Open(filepath string) (err error) {
	db.File, err = os.OpenFile(filepath, os.O_RDWR|os.O_CREATE, 0644)
	if err != nil {
		return
	}

	db.Path = filepath

	return
}

func (db *Database) resetSeek() error {
	_, err := db.File.Seek(0, 0)

	return err
}

func (db *Database) Read() error {
	db.resetSeek()

	decoder := gob.NewDecoder(db.File)
	return decoder.Decode(&db.NoteList)
}

func (db *Database) Write() error {
	db.resetSeek()

	encoder := gob.NewEncoder(db.File)
	return encoder.Encode(db.NoteList)
}

func (db *Database) Close() error {
	db.Path = ""
	db.NoteList = []Note{}

	return db.File.Close()
}

func (db *Database) Append(note Note) {
	db.NoteList = append(db.NoteList, note)
}

func (db *Database) Remove(index int) {
	db.NoteList = slices.Delete(db.NoteList, index, index+1)
}

func Init() (db Database) {
	home, err := os.UserHomeDir()
	if err != nil {
		panic(err)
	}

	if err := db.Open(home + "/cliper_global_notes.bin"); err != nil {
		panic(err)
	}

	return
}
