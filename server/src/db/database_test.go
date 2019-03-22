package db

import (
	"testing"

	_ "github.com/lib/pq"
)

func TestCreateTable(t *testing.T) {
	type args struct {
		mesage Message
	}
	tests := []struct {
		name string
		args args
	}{
		{"Test1", args{Message{}}},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			CreateTable(tt.args.mesage)
		})
	}
}

func TestExistTable(t *testing.T) {
	type args struct {
		schema string
		table  string
	}
	tests := []struct {
		name string
		args args
		want bool
	}{
		{"Test Exist Table", args{"publish", "message"}, false},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if got := ExistTable(tt.args.schema, tt.args.table); got != tt.want {
				t.Errorf("ExistTable() = %v, want %v", got, tt.want)
			}
		})
	}
}
