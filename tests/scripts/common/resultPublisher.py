from tabulate import tabulate

def statusToText(status):
	if status == True:
		return 'pass'
	return 'fail'

def createEmptyTable(tests, targets):
    out = {}

    for test in tests:
        out[test] = {}
        for target in targets:
            out[test][target] = 'configuration missing'
    return out

def showResult(data):
    tests = set()
    targets = set()
    for x in data:
        targets.add(x['target'])
        tests.add(x['testName'])
#    print tests
#    print targets
    
    table = createEmptyTable(tests, targets)
    for result in data:
        table[result['testName']][result['target']] = statusToText(result['status'])   

    printableTable = []
    for test in tests:                
        tmp = [test]
        for target in targets:
            tmp.append(table[test][target])
        printableTable.append(tmp)

    header = ['test Name']
    for x in targets:
        header.append(x)
    print '\n'
    print tabulate(printableTable, headers=header, tablefmt='orgtbl')
    print '\n'
