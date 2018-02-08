package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"strconv"
	"strings"
	"time"
	"crypto/sha1"
	"io"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
)

// SimpleChaincode example simple Chaincode implementation
type SimpleChaincode struct {
}

type ehr struct {
	TransType 		string 		`json:"transType"` 		//交易类型，1
	TransCode 		string 		`json:"transCode"` 		//交易代码，1-上链，2-
	Orgid       	string 		`json:"orgid"`    		//所有者机构
	Mspid      		string 		`json:"mspid"`			//所有者ID
	Authids      	string 		`json:"authids"`			//授权者ID或多个ID
	Resource		string 		`json:"resource"`		//资源属性
	Validdate       time.Time   `json:"validdate"`  		//授权有效天数
}

// ===================================================================================
// Main
// ===================================================================================
func main() {
	err := shim.Start(new(SimpleChaincode))
	if err != nil {
		fmt.Printf("Error starting Simple chaincode: %s", err)
	}
}

// Init initializes chaincode
// ===========================
func (t *SimpleChaincode) Init(stub shim.ChaincodeStubInterface) pb.Response {
	return shim.Success(nil)
}

// Invoke - Our entry point for Invocations
// ========================================
func (t *SimpleChaincode) Invoke(stub shim.ChaincodeStubInterface) pb.Response {
	function, args := stub.GetFunctionAndParameters()
	fmt.Println("invoke is running " + function)

	// Handle different functions
	if function == "initEhr" { //create a new ehr
		return t.initEhr(stub, args)
	} else if function == "queryEhrsByOwnerOrg" { //change owner of a specific ehr
		return t.queryEhrsByOwnerOrg(stub, args)
	} else if function == "queryEhrsByOwnerMsp" { //transfer all ehrs of a certain color
		return t.queryEhrsByOwnerMsp(stub, args)
	} else if function == "queryEhrsByAuthids" { //delete a ehr
		return t.queryEhrsByAuthids(stub, args)
	} else if function == "queryEhrsByTrans" { //read a ehr
		return t.queryEhrsByTrans(stub, args)
	} else if function == "readInfo" { 
		return t.readInfo(stub, args)
	}

	fmt.Println("invoke did not find func: " + function) //error
	return shim.Error("Received unknown function invocation")
}

// ============================================================
// initEhr - create a new ehr, store into chaincode state
// ============================================================
func (t *SimpleChaincode) initEhr(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var err error

	if len(args) != 7 {
		return shim.Error("Incorrect number of arguments. Expecting 7")
	}

	// ==== Input sanitation ====
	fmt.Println("- start init ehr")
	if len(args[0]) <= 0 {
		return shim.Error("1st argument must be a non-empty string")
	}
	if len(args[1]) <= 0 {
		return shim.Error("2nd argument must be a non-empty string")
	}
	if len(args[2]) <= 0 {
		return shim.Error("3rd argument must be a non-empty string")
	}
	if len(args[3]) <= 0 {
		return shim.Error("4th argument must be a non-empty string")
	}
	if len(args[4]) <= 0 {
		return shim.Error("5nd argument must be a non-empty string")
	}
	if len(args[5]) <= 0 {
		return shim.Error("6rd argument must be a non-empty string")
	}
	if len(args[6]) <= 0 {
		return shim.Error("7th argument must be a non-empty string")
	}

	transType := strings.ToLower(args[0])
	transCode := strings.ToLower(args[1])
	orgid := strings.ToLower(args[2])
	mspid := strings.ToLower(args[3])
	authids := strings.ToLower(args[4])
	resource := strings.ToLower(args[5])
	validdate, err := time.Parse("2006-01-02 15:04:05", args[6])

	if err != nil {
		return shim.Error("7rd argument must be a date string, for example : 2011-11-11 11:11:11")
	}

	// 主键为机构ID+用户ID+资源路径+有效期Unix时间的HASH值
	//keyValue := hash(orgid+mspid+resource+ strconv.FormatInt(validdate.Unix(),10))
	keyValue := args[0]

	// ==== Check if ehr already exists ====
	ehrAsBytes, err := stub.GetState(keyValue)
	if err != nil {
		return shim.Error("Failed to get ehr: " + err.Error())
	} else if ehrAsBytes != nil {
		fmt.Println("This ehr resource hash already exists! ")
		return shim.Error("This ehr resource already exists! ")
	}

	// ==== Create ehr object to JSON ====
	ehr := &ehr{transType,transCode,orgid,mspid, authids,resource, validdate}
	ehrJSONasBytes, err := json.Marshal(ehr)
	if err != nil {
		return shim.Error(err.Error())
	}
	//Alternatively, build the ehr json string manually if you don't want to use struct marshalling
	//ehrJSONasString := `{"docType":"Ehr",  "name": "` + ehrName + `", "color": "` + color + `", "size": ` + strconv.Itoa(size) + `, "owner": "` + owner + `"}`
	//ehrJSONasBytes := []byte(str)

	// === Save ehr to state ===
	err = stub.PutState(keyValue, ehrJSONasBytes)
	if err != nil {
		return shim.Error(err.Error())
	}

	//  ==== Index the ehr to enable color-based range queries, e.g. return all blue ehrs ====
	//  An 'index' is a normal key/value entry in state.
	//  The key is a composite key, with the elements that you want to range query on listed first.
	//  In our case, the composite key is based on indexName~color~name.
	//  This will enable very efficient state range queries based on composite keys matching indexName~color~*

	//ownerIndexName := "orgid~mspid"
	//ownerIndexKey, err := stub.CreateCompositeKey(ownerIndexName, []string{ehr.Orgid, ehr.Mspid})
	//if err != nil {
	//	return shim.Error(err.Error())
	//}
	////  Save index entry to state. Only the key name is needed, no need to store a duplicate copy of the ehr.
	////  Note - passing a 'nil' value will effectively delete the key from state, therefore we pass null character as value
	//ownerValue := []byte{0x00}
	//stub.PutState(ownerIndexKey, ownerValue)
	//
	//authIndexName := "authids"
	//authIndexKey, err := stub.CreateCompositeKey(authIndexName, []string{ehr.Authids})
	//if err != nil {
	//	return shim.Error(err.Error())
	//}
	////  Save index entry to state. Only the key name is needed, no need to store a duplicate copy of the ehr.
	////  Note - passing a 'nil' value will effectively delete the key from state, therefore we pass null character as value
	//authValue := []byte{0x00}
	//stub.PutState(authIndexKey, authValue)

	// ==== Ehr saved and indexed. Return success ====
	fmt.Println("- end init ehr")
	return shim.Success(nil)
}

func (t *SimpleChaincode) readInfo(stub shim.ChaincodeStubInterface, args []string) pb.Response {

	var name, jsonResp string
	var err error

	if len(args) != 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}

	name = args[0]
	valAsbytes, err := stub.GetState(name) //get the marble from chaincode state
	if err != nil {
		jsonResp = "{\"Error\":\"Failed to get state for " + name + "\"}"
		return shim.Error(jsonResp)
	} else if valAsbytes == nil {
		jsonResp = "{\"Error\":\"Marble does not exist: " + name + "\"}"
		return shim.Error(jsonResp)
	}

	return shim.Success(valAsbytes)
}

// =========================================================================================
// 使用机构ID查询
// =========================================================================================
func (t *SimpleChaincode) queryEhrsByOwnerOrg(stub shim.ChaincodeStubInterface, args []string) pb.Response {

	if len(args) < 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}

	orgid := strings.ToLower(args[0])

	queryString := fmt.Sprintf("{\"selector\":{\"orgid\":\"%s\"}}", orgid)

	queryResults, err := getQueryResultForQueryString(stub, queryString)
	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(queryResults)
}

// =========================================================================================
// 使用人员ID查询
// =========================================================================================
func (t *SimpleChaincode) queryEhrsByOwnerMsp(stub shim.ChaincodeStubInterface, args []string) pb.Response {

	if len(args) < 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}

	mspid := strings.ToLower(args[0])

	queryString := fmt.Sprintf("{\"selector\":{\"mspid\":\"%s\"}}", mspid)

	queryResults, err := getQueryResultForQueryString(stub, queryString)
	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(queryResults)
}


// =========================================================================================
// 使用授权ID查询
// =========================================================================================
func (t *SimpleChaincode) queryEhrsByAuthids(stub shim.ChaincodeStubInterface, args []string) pb.Response {

	if len(args) < 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}

	authids := strings.ToLower(args[0])

	queryString := fmt.Sprintf("{\"selector\":{\"authids\":\"%s\"}}", authids)

	queryResults, err := getQueryResultForQueryString(stub, queryString)
	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(queryResults)
}

// =========================================================================================
// 使用交易类型查询
// =========================================================================================
func (t *SimpleChaincode) queryEhrsByTrans(stub shim.ChaincodeStubInterface, args []string) pb.Response {

	if len(args) < 2 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}

	authids := strings.ToLower(args[0])

	//queryString := fmt.Sprintf("{\"selector\":{\"authids\":\"%s\"}}", authids)
	queryString := fmt.Sprintf("{\"selector\":{\"docType\":\"marble\",\"authids\":\"%s\"}}", authids)

	queryResults, err := getQueryResultForQueryString(stub, queryString)
	if err != nil {
		return shim.Error(err.Error())
	}
	return shim.Success(queryResults)
}

// =========================================================================================
// getQueryResultForQueryString executes the passed in query string.
// Result set is built and returned as a byte array containing the JSON results.
// =========================================================================================
func getQueryResultForQueryString(stub shim.ChaincodeStubInterface, queryString string) ([]byte, error) {

    fmt.Printf("- getQueryResultForQueryString queryString:\n%s\n", queryString)

	resultsIterator, err := stub.GetQueryResult(queryString)
	if err != nil {
		return nil, err
	}
	defer resultsIterator.Close()

	// buffer is a JSON array containing QueryRecords
	var buffer bytes.Buffer
	buffer.WriteString("[")

	bArrayMemberAlreadyWritten := false
	for resultsIterator.HasNext() {
		queryResponse, err := resultsIterator.Next()
		if err != nil {
			return nil, err
		}
		// Add a comma before array members, suppress it for the first array member
		if bArrayMemberAlreadyWritten == true {
			buffer.WriteString(",")
		}
		buffer.WriteString("{\"Key\":")
		buffer.WriteString("\"")
		buffer.WriteString(queryResponse.Key)
		buffer.WriteString("\"")

		buffer.WriteString(", \"Record\":")
		// Record is a JSON object, so we write as-is
		buffer.WriteString(string(queryResponse.Value))
		buffer.WriteString("}")
		bArrayMemberAlreadyWritten = true
	}
	buffer.WriteString("]")
	
	fmt.Printf("- getQueryResultForQueryString queryResult:\n%s\n", buffer.String())

	return buffer.Bytes(), nil
}

func hash(data string) string {
	t := sha1.New()
	io.WriteString(t, data)
	return fmt.Sprintf("%x", t.Sum(nil))
}
