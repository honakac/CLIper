package cmds

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

const TIMESTAMP_FORMAT string = "2006-01-02 15:04:05"
