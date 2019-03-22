package db

import (
	"database/sql"
	"fmt"
	"log"
	"os"
	"reflect"
	"time"

	_ "github.com/lib/pq"
)

const driver = "postgres"

const INSERT = "INSERT INTO MESSAGE(Time,Topic,FarmName,Type,DeviceId,Value) VALUES(?,?,?,?,?,?) "

const (
	dbhost = "DBHOST"
	dbport = "DBPORT"
	dbuser = "DBUSER"
	dbpass = "DBPASS"
	dbname = "DBNAME"
)

var DB *sql.DB

type Message struct {
	Time     time.Time `column:"time"`
	Topic    string    `column:"topic"`
	FarmName string    `column:"farm_name"`
	Type     string    `column:"type"`
	DeviceId string    `column:"device_id`
	Value    string    `column:"value"`
}

func init() {
	config := dbConfig()
	var err error
	// psqlInfo := fmt.Sprintf("host=%s port=%s user=%s password=%s dbname=%s sslmode=disable",
	// 	config[dbhost], config[dbport], config[dbuser], config[dbpass], config[dbname])

	psqlInfo := fmt.Sprintf("host=%s port=%s user=%s dbname=%s sslmode=disable",
		config[dbhost], config[dbport], config[dbuser], config[dbname])
	fmt.Println(psqlInfo)
	DB, err = sql.Open("postgres", psqlInfo)
	if err != nil {
		panic(err)
	}
	err = DB.Ping()
	if err != nil {
		panic(err)
	}
	fmt.Println("Successfully connected!")
}

var DEFAULT_DBHOST = "localhost"
var DEFAULT_DBPORT = "5432"
var DEFAULT_DBUSER = "postgres"
var DEFAULT_DBPASS = ""
var DEFAULT_DBNAME = "farm"

func dbConfig() map[string]string {
	conf := make(map[string]string)
	host, ok := os.LookupEnv(dbhost)
	if !ok {
		host = DEFAULT_DBHOST
	}
	port, ok := os.LookupEnv(dbport)
	if !ok {
		port = DEFAULT_DBPORT
	}
	user, ok := os.LookupEnv(dbuser)
	if !ok {
		user = DEFAULT_DBUSER
	}
	password, ok := os.LookupEnv(dbpass)
	if !ok {
		password = DEFAULT_DBPASS
	}
	name, ok := os.LookupEnv(dbname)
	if !ok {
		name = DEFAULT_DBNAME
	}
	conf[dbhost] = host
	conf[dbport] = port
	conf[dbuser] = user
	conf[dbpass] = password
	conf[dbname] = name
	return conf
}

func CreateTable(mesage Message) {
	typ := reflect.TypeOf(mesage)
	for i := 0; i < typ.NumField(); i++ {
		valueField := typ.Field(i)
		fmt.Printf("%v %v %v \n", valueField.Name, valueField.Type, valueField.Tag.Get("column"))

		// fmt.Printf("Field Name: %s,\t Field Value: %v,\t Tag Value: %s\n", typeField.String(), valueField, tag.Get("tag_name"))
	}
}

const TABLE = "CREATE TABLE  $1"

func createTable(name string) {
	DB.Exec(TABLE, name)
}

var IsTableExist = `SELECT EXISTS (
	SELECT 1
	FROM   information_schema.tables 
	WHERE  table_schema = $1
	AND    table_name = $2
	);`

func ExistTable(schema, table string) bool {
	stmt, err := DB.Prepare(IsTableExist)
	if err != nil {
		panic(err)
	}
	defer stmt.Close()

	rows, err := stmt.Query(schema, table)
	if err != nil {
		panic(err)
	}
	defer rows.Close()
	exist := false
	for rows.Next() {
		err := rows.Scan(&exist)
		if err != nil {
			log.Fatal(err)
		}
	}
	return exist
}

func Save(message Message) {

	tx, err := DB.Begin()
	if err != nil {
		log.Fatal(err)
	}

	stmt, err := tx.Prepare(INSERT)
	if err != nil {
		tx.Rollback()
	}
	defer stmt.Close()
	_, err = stmt.Exec(message.Time, message.Topic, message.FarmName, message.Type, message.DeviceId, message.Value)

	if err != nil {
		tx.Rollback()
	} else {
		tx.Commit()
	}
}
