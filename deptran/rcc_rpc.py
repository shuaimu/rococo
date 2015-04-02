import os
from simplerpc.marshal import Marshal
from simplerpc.future import Future

RequestHeader = Marshal.reg_type('RequestHeader', [('t_type', 'rrr::i32'), ('p_type', 'rrr::i32'), ('cid', 'rrr::i32'), ('tid', 'rrr::i64'), ('pid', 'rrr::i64')])

BatchRequestHeader = Marshal.reg_type('BatchRequestHeader', [('t_type', 'rrr::i32'), ('cid', 'rrr::i32'), ('tid', 'rrr::i64'), ('num_piece', 'rrr::i32'), ('expected_output_size', 'rrr::i32')])

BatchChopStartResponse = Marshal.reg_type('BatchChopStartResponse', [('is_defers', 'std::vector<rrr::i8>'), ('outputs', 'std::vector<std::vector<Value>>'), ('gra_m', 'GraphMarshaler')])

ChopStartResponse = Marshal.reg_type('ChopStartResponse', [('is_defered', 'rrr::i8'), ('output', 'std::vector<Value>'), ('gra_m', 'GraphMarshaler'), ('read_only', 'std::string')])

ChopFinishRequest = Marshal.reg_type('ChopFinishRequest', [('txn_id', 'rrr::i64'), ('gra', 'Graph<TxnInfo>'), ('read_only', 'std::string')])

ChopFinishResponse = Marshal.reg_type('ChopFinishResponse', [('res', 'rrr::i32'), ('outputs', 'std::vector<std::pair<RequestHeader, std::vector<Value>>>')])

CollectFinishResponse = Marshal.reg_type('CollectFinishResponse', [('gra_m', 'GraphMarshaler')])

ValueTimesPair = Marshal.reg_type('ValueTimesPair', [('value', 'rrr::i64'), ('times', 'rrr::i64')])

TxnInfoRes = Marshal.reg_type('TxnInfoRes', [('start_txn', 'rrr::i32'), ('total_txn', 'rrr::i32'), ('total_try', 'rrr::i32'), ('commit_txn', 'rrr::i32'), ('this_latency', 'std::vector<double>'), ('last_latency', 'std::vector<double>'), ('attempt_latency', 'std::vector<double>'), ('interval_latency', 'std::vector<double>'), ('num_try', 'std::vector<rrr::i32>')])

ServerResponse = Marshal.reg_type('ServerResponse', [('statistics', 'std::map<std::string, ValueTimesPair>'), ('cpu_util', 'double'), ('r_cnt_sum', 'rrr::i64'), ('r_cnt_num', 'rrr::i64'), ('r_sz_sum', 'rrr::i64'), ('r_sz_num', 'rrr::i64')])

ClientResponse = Marshal.reg_type('ClientResponse', [('txn_info', 'std::map<rrr::i32, TxnInfoRes>'), ('run_sec', 'rrr::i64'), ('run_nsec', 'rrr::i64'), ('period_sec', 'rrr::i64'), ('period_nsec', 'rrr::i64'), ('is_finish', 'rrr::i32'), ('n_asking', 'rrr::i64')])

class RococoService(object):
    START_PIE = 0x3a743129
    PREPARE_TXN = 0x1b0a7970
    COMMIT_TXN = 0x24e1ca21
    ABORT_TXN = 0x1709d704
    BATCH_START_PIE = 0x5b5f4cfd
    NAIVE_BATCH_START_PIE = 0x28bb75e4
    RCC_START_PIE = 0x3cfba832
    RCC_FINISH_TXN = 0x3f6b3d9f
    RCC_ASK_TXN = 0x43d03e15
    RCC_RO_START_PIE = 0x1bd0ee8d
    RCC_BATCH_START_PIE = 0x220786b9
    RPC_NULL = 0x28b9b19b

    __input_type_info__ = {
        'start_pie': ['RequestHeader','std::vector<Value>','rrr::i32'],
        'prepare_txn': ['rrr::i64','std::vector<rrr::i32>'],
        'commit_txn': ['rrr::i64'],
        'abort_txn': ['rrr::i64'],
        'batch_start_pie': ['BatchRequestHeader','std::vector<Value>'],
        'naive_batch_start_pie': ['std::vector<RequestHeader>','std::vector<std::vector<Value>>','std::vector<rrr::i32>'],
        'rcc_start_pie': ['RequestHeader','std::vector<Value>'],
        'rcc_finish_txn': ['ChopFinishRequest'],
        'rcc_ask_txn': ['rrr::i64'],
        'rcc_ro_start_pie': ['RequestHeader','std::vector<Value>'],
        'rcc_batch_start_pie': ['std::vector<RequestHeader>','std::vector<std::vector<Value>>'],
        'rpc_null': [],
    }

    __output_type_info__ = {
        'start_pie': ['rrr::i32','std::vector<Value>'],
        'prepare_txn': ['rrr::i32'],
        'commit_txn': ['rrr::i32'],
        'abort_txn': ['rrr::i32'],
        'batch_start_pie': ['rrr::i32','std::vector<Value>'],
        'naive_batch_start_pie': ['std::vector<rrr::i32>','std::vector<std::vector<Value>>'],
        'rcc_start_pie': ['ChopStartResponse'],
        'rcc_finish_txn': ['ChopFinishResponse'],
        'rcc_ask_txn': ['CollectFinishResponse'],
        'rcc_ro_start_pie': ['std::vector<Value>'],
        'rcc_batch_start_pie': ['BatchChopStartResponse'],
        'rpc_null': [],
    }

    def __bind_helper__(self, func):
        def f(*args):
            return getattr(self, func.__name__)(*args)
        return f

    def __reg_to__(self, server):
        server.__reg_func__(RococoService.START_PIE, self.__bind_helper__(self.start_pie), ['RequestHeader','std::vector<Value>','rrr::i32'], ['rrr::i32','std::vector<Value>'])
        server.__reg_func__(RococoService.PREPARE_TXN, self.__bind_helper__(self.prepare_txn), ['rrr::i64','std::vector<rrr::i32>'], ['rrr::i32'])
        server.__reg_func__(RococoService.COMMIT_TXN, self.__bind_helper__(self.commit_txn), ['rrr::i64'], ['rrr::i32'])
        server.__reg_func__(RococoService.ABORT_TXN, self.__bind_helper__(self.abort_txn), ['rrr::i64'], ['rrr::i32'])
        server.__reg_func__(RococoService.BATCH_START_PIE, self.__bind_helper__(self.batch_start_pie), ['BatchRequestHeader','std::vector<Value>'], ['rrr::i32','std::vector<Value>'])
        server.__reg_func__(RococoService.NAIVE_BATCH_START_PIE, self.__bind_helper__(self.naive_batch_start_pie), ['std::vector<RequestHeader>','std::vector<std::vector<Value>>','std::vector<rrr::i32>'], ['std::vector<rrr::i32>','std::vector<std::vector<Value>>'])
        server.__reg_func__(RococoService.RCC_START_PIE, self.__bind_helper__(self.rcc_start_pie), ['RequestHeader','std::vector<Value>'], ['ChopStartResponse'])
        server.__reg_func__(RococoService.RCC_FINISH_TXN, self.__bind_helper__(self.rcc_finish_txn), ['ChopFinishRequest'], ['ChopFinishResponse'])
        server.__reg_func__(RococoService.RCC_ASK_TXN, self.__bind_helper__(self.rcc_ask_txn), ['rrr::i64'], ['CollectFinishResponse'])
        server.__reg_func__(RococoService.RCC_RO_START_PIE, self.__bind_helper__(self.rcc_ro_start_pie), ['RequestHeader','std::vector<Value>'], ['std::vector<Value>'])
        server.__reg_func__(RococoService.RCC_BATCH_START_PIE, self.__bind_helper__(self.rcc_batch_start_pie), ['std::vector<RequestHeader>','std::vector<std::vector<Value>>'], ['BatchChopStartResponse'])
        server.__reg_func__(RococoService.RPC_NULL, self.__bind_helper__(self.rpc_null), [], [])

    def start_pie(__self__, header, input, output_size):
        raise NotImplementedError('subclass RococoService and implement your own start_pie function')

    def prepare_txn(__self__, tid, sids):
        raise NotImplementedError('subclass RococoService and implement your own prepare_txn function')

    def commit_txn(__self__, tid):
        raise NotImplementedError('subclass RococoService and implement your own commit_txn function')

    def abort_txn(__self__, tid):
        raise NotImplementedError('subclass RococoService and implement your own abort_txn function')

    def batch_start_pie(__self__, batch_header, input):
        raise NotImplementedError('subclass RococoService and implement your own batch_start_pie function')

    def naive_batch_start_pie(__self__, header, input, output_size):
        raise NotImplementedError('subclass RococoService and implement your own naive_batch_start_pie function')

    def rcc_start_pie(__self__, header, input):
        raise NotImplementedError('subclass RococoService and implement your own rcc_start_pie function')

    def rcc_finish_txn(__self__, req):
        raise NotImplementedError('subclass RococoService and implement your own rcc_finish_txn function')

    def rcc_ask_txn(__self__, txn_id):
        raise NotImplementedError('subclass RococoService and implement your own rcc_ask_txn function')

    def rcc_ro_start_pie(__self__, header, input):
        raise NotImplementedError('subclass RococoService and implement your own rcc_ro_start_pie function')

    def rcc_batch_start_pie(__self__, headers, inputs):
        raise NotImplementedError('subclass RococoService and implement your own rcc_batch_start_pie function')

    def rpc_null(__self__):
        raise NotImplementedError('subclass RococoService and implement your own rpc_null function')

class RococoProxy(object):
    def __init__(self, clnt):
        self.__clnt__ = clnt

    def async_start_pie(__self__, header, input, output_size):
        return __self__.__clnt__.async_call(RococoService.START_PIE, [header, input, output_size], RococoService.__input_type_info__['start_pie'], RococoService.__output_type_info__['start_pie'])

    def async_prepare_txn(__self__, tid, sids):
        return __self__.__clnt__.async_call(RococoService.PREPARE_TXN, [tid, sids], RococoService.__input_type_info__['prepare_txn'], RococoService.__output_type_info__['prepare_txn'])

    def async_commit_txn(__self__, tid):
        return __self__.__clnt__.async_call(RococoService.COMMIT_TXN, [tid], RococoService.__input_type_info__['commit_txn'], RococoService.__output_type_info__['commit_txn'])

    def async_abort_txn(__self__, tid):
        return __self__.__clnt__.async_call(RococoService.ABORT_TXN, [tid], RococoService.__input_type_info__['abort_txn'], RococoService.__output_type_info__['abort_txn'])

    def async_batch_start_pie(__self__, batch_header, input):
        return __self__.__clnt__.async_call(RococoService.BATCH_START_PIE, [batch_header, input], RococoService.__input_type_info__['batch_start_pie'], RococoService.__output_type_info__['batch_start_pie'])

    def async_naive_batch_start_pie(__self__, header, input, output_size):
        return __self__.__clnt__.async_call(RococoService.NAIVE_BATCH_START_PIE, [header, input, output_size], RococoService.__input_type_info__['naive_batch_start_pie'], RococoService.__output_type_info__['naive_batch_start_pie'])

    def async_rcc_start_pie(__self__, header, input):
        return __self__.__clnt__.async_call(RococoService.RCC_START_PIE, [header, input], RococoService.__input_type_info__['rcc_start_pie'], RococoService.__output_type_info__['rcc_start_pie'])

    def async_rcc_finish_txn(__self__, req):
        return __self__.__clnt__.async_call(RococoService.RCC_FINISH_TXN, [req], RococoService.__input_type_info__['rcc_finish_txn'], RococoService.__output_type_info__['rcc_finish_txn'])

    def async_rcc_ask_txn(__self__, txn_id):
        return __self__.__clnt__.async_call(RococoService.RCC_ASK_TXN, [txn_id], RococoService.__input_type_info__['rcc_ask_txn'], RococoService.__output_type_info__['rcc_ask_txn'])

    def async_rcc_ro_start_pie(__self__, header, input):
        return __self__.__clnt__.async_call(RococoService.RCC_RO_START_PIE, [header, input], RococoService.__input_type_info__['rcc_ro_start_pie'], RococoService.__output_type_info__['rcc_ro_start_pie'])

    def async_rcc_batch_start_pie(__self__, headers, inputs):
        return __self__.__clnt__.async_call(RococoService.RCC_BATCH_START_PIE, [headers, inputs], RococoService.__input_type_info__['rcc_batch_start_pie'], RococoService.__output_type_info__['rcc_batch_start_pie'])

    def async_rpc_null(__self__):
        return __self__.__clnt__.async_call(RococoService.RPC_NULL, [], RococoService.__input_type_info__['rpc_null'], RococoService.__output_type_info__['rpc_null'])

    def sync_start_pie(__self__, header, input, output_size):
        __result__ = __self__.__clnt__.sync_call(RococoService.START_PIE, [header, input, output_size], RococoService.__input_type_info__['start_pie'], RococoService.__output_type_info__['start_pie'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_prepare_txn(__self__, tid, sids):
        __result__ = __self__.__clnt__.sync_call(RococoService.PREPARE_TXN, [tid, sids], RococoService.__input_type_info__['prepare_txn'], RococoService.__output_type_info__['prepare_txn'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_commit_txn(__self__, tid):
        __result__ = __self__.__clnt__.sync_call(RococoService.COMMIT_TXN, [tid], RococoService.__input_type_info__['commit_txn'], RococoService.__output_type_info__['commit_txn'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_abort_txn(__self__, tid):
        __result__ = __self__.__clnt__.sync_call(RococoService.ABORT_TXN, [tid], RococoService.__input_type_info__['abort_txn'], RococoService.__output_type_info__['abort_txn'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_batch_start_pie(__self__, batch_header, input):
        __result__ = __self__.__clnt__.sync_call(RococoService.BATCH_START_PIE, [batch_header, input], RococoService.__input_type_info__['batch_start_pie'], RococoService.__output_type_info__['batch_start_pie'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_naive_batch_start_pie(__self__, header, input, output_size):
        __result__ = __self__.__clnt__.sync_call(RococoService.NAIVE_BATCH_START_PIE, [header, input, output_size], RococoService.__input_type_info__['naive_batch_start_pie'], RococoService.__output_type_info__['naive_batch_start_pie'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_rcc_start_pie(__self__, header, input):
        __result__ = __self__.__clnt__.sync_call(RococoService.RCC_START_PIE, [header, input], RococoService.__input_type_info__['rcc_start_pie'], RococoService.__output_type_info__['rcc_start_pie'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_rcc_finish_txn(__self__, req):
        __result__ = __self__.__clnt__.sync_call(RococoService.RCC_FINISH_TXN, [req], RococoService.__input_type_info__['rcc_finish_txn'], RococoService.__output_type_info__['rcc_finish_txn'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_rcc_ask_txn(__self__, txn_id):
        __result__ = __self__.__clnt__.sync_call(RococoService.RCC_ASK_TXN, [txn_id], RococoService.__input_type_info__['rcc_ask_txn'], RococoService.__output_type_info__['rcc_ask_txn'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_rcc_ro_start_pie(__self__, header, input):
        __result__ = __self__.__clnt__.sync_call(RococoService.RCC_RO_START_PIE, [header, input], RococoService.__input_type_info__['rcc_ro_start_pie'], RococoService.__output_type_info__['rcc_ro_start_pie'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_rcc_batch_start_pie(__self__, headers, inputs):
        __result__ = __self__.__clnt__.sync_call(RococoService.RCC_BATCH_START_PIE, [headers, inputs], RococoService.__input_type_info__['rcc_batch_start_pie'], RococoService.__output_type_info__['rcc_batch_start_pie'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_rpc_null(__self__):
        __result__ = __self__.__clnt__.sync_call(RococoService.RPC_NULL, [], RococoService.__input_type_info__['rpc_null'], RococoService.__output_type_info__['rpc_null'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

class ServerControlService(object):
    SERVER_SHUTDOWN = 0x18dbfed8
    SERVER_READY = 0x31436c6d
    SERVER_HEART_BEAT_WITH_DATA = 0x59acd9da
    SERVER_HEART_BEAT = 0x3cd34ed0

    __input_type_info__ = {
        'server_shutdown': [],
        'server_ready': [],
        'server_heart_beat_with_data': [],
        'server_heart_beat': [],
    }

    __output_type_info__ = {
        'server_shutdown': [],
        'server_ready': ['rrr::i32'],
        'server_heart_beat_with_data': ['ServerResponse'],
        'server_heart_beat': [],
    }

    def __bind_helper__(self, func):
        def f(*args):
            return getattr(self, func.__name__)(*args)
        return f

    def __reg_to__(self, server):
        server.__reg_func__(ServerControlService.SERVER_SHUTDOWN, self.__bind_helper__(self.server_shutdown), [], [])
        server.__reg_func__(ServerControlService.SERVER_READY, self.__bind_helper__(self.server_ready), [], ['rrr::i32'])
        server.__reg_func__(ServerControlService.SERVER_HEART_BEAT_WITH_DATA, self.__bind_helper__(self.server_heart_beat_with_data), [], ['ServerResponse'])
        server.__reg_func__(ServerControlService.SERVER_HEART_BEAT, self.__bind_helper__(self.server_heart_beat), [], [])

    def server_shutdown(__self__):
        raise NotImplementedError('subclass ServerControlService and implement your own server_shutdown function')

    def server_ready(__self__):
        raise NotImplementedError('subclass ServerControlService and implement your own server_ready function')

    def server_heart_beat_with_data(__self__):
        raise NotImplementedError('subclass ServerControlService and implement your own server_heart_beat_with_data function')

    def server_heart_beat(__self__):
        raise NotImplementedError('subclass ServerControlService and implement your own server_heart_beat function')

class ServerControlProxy(object):
    def __init__(self, clnt):
        self.__clnt__ = clnt

    def async_server_shutdown(__self__):
        return __self__.__clnt__.async_call(ServerControlService.SERVER_SHUTDOWN, [], ServerControlService.__input_type_info__['server_shutdown'], ServerControlService.__output_type_info__['server_shutdown'])

    def async_server_ready(__self__):
        return __self__.__clnt__.async_call(ServerControlService.SERVER_READY, [], ServerControlService.__input_type_info__['server_ready'], ServerControlService.__output_type_info__['server_ready'])

    def async_server_heart_beat_with_data(__self__):
        return __self__.__clnt__.async_call(ServerControlService.SERVER_HEART_BEAT_WITH_DATA, [], ServerControlService.__input_type_info__['server_heart_beat_with_data'], ServerControlService.__output_type_info__['server_heart_beat_with_data'])

    def async_server_heart_beat(__self__):
        return __self__.__clnt__.async_call(ServerControlService.SERVER_HEART_BEAT, [], ServerControlService.__input_type_info__['server_heart_beat'], ServerControlService.__output_type_info__['server_heart_beat'])

    def sync_server_shutdown(__self__):
        __result__ = __self__.__clnt__.sync_call(ServerControlService.SERVER_SHUTDOWN, [], ServerControlService.__input_type_info__['server_shutdown'], ServerControlService.__output_type_info__['server_shutdown'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_server_ready(__self__):
        __result__ = __self__.__clnt__.sync_call(ServerControlService.SERVER_READY, [], ServerControlService.__input_type_info__['server_ready'], ServerControlService.__output_type_info__['server_ready'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_server_heart_beat_with_data(__self__):
        __result__ = __self__.__clnt__.sync_call(ServerControlService.SERVER_HEART_BEAT_WITH_DATA, [], ServerControlService.__input_type_info__['server_heart_beat_with_data'], ServerControlService.__output_type_info__['server_heart_beat_with_data'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_server_heart_beat(__self__):
        __result__ = __self__.__clnt__.sync_call(ServerControlService.SERVER_HEART_BEAT, [], ServerControlService.__input_type_info__['server_heart_beat'], ServerControlService.__output_type_info__['server_heart_beat'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

class ClientControlService(object):
    CLIENT_GET_TXN_NAMES = 0x60cdbcc4
    CLIENT_SHUTDOWN = 0x3480baca
    CLIENT_FORCE_STOP = 0x19a6b162
    CLIENT_RESPONSE = 0x661cb1ac
    CLIENT_READY = 0x6e321e29
    CLIENT_READY_BLOCK = 0x48a23cb9
    CLIENT_START = 0x56d10dff

    __input_type_info__ = {
        'client_get_txn_names': [],
        'client_shutdown': [],
        'client_force_stop': [],
        'client_response': [],
        'client_ready': [],
        'client_ready_block': [],
        'client_start': [],
    }

    __output_type_info__ = {
        'client_get_txn_names': ['std::map<rrr::i32, std::string>'],
        'client_shutdown': [],
        'client_force_stop': [],
        'client_response': ['ClientResponse'],
        'client_ready': ['rrr::i32'],
        'client_ready_block': ['rrr::i32'],
        'client_start': [],
    }

    def __bind_helper__(self, func):
        def f(*args):
            return getattr(self, func.__name__)(*args)
        return f

    def __reg_to__(self, server):
        server.__reg_func__(ClientControlService.CLIENT_GET_TXN_NAMES, self.__bind_helper__(self.client_get_txn_names), [], ['std::map<rrr::i32, std::string>'])
        server.__reg_func__(ClientControlService.CLIENT_SHUTDOWN, self.__bind_helper__(self.client_shutdown), [], [])
        server.__reg_func__(ClientControlService.CLIENT_FORCE_STOP, self.__bind_helper__(self.client_force_stop), [], [])
        server.__reg_func__(ClientControlService.CLIENT_RESPONSE, self.__bind_helper__(self.client_response), [], ['ClientResponse'])
        server.__reg_func__(ClientControlService.CLIENT_READY, self.__bind_helper__(self.client_ready), [], ['rrr::i32'])
        server.__reg_func__(ClientControlService.CLIENT_READY_BLOCK, self.__bind_helper__(self.client_ready_block), [], ['rrr::i32'])
        server.__reg_func__(ClientControlService.CLIENT_START, self.__bind_helper__(self.client_start), [], [])

    def client_get_txn_names(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_get_txn_names function')

    def client_shutdown(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_shutdown function')

    def client_force_stop(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_force_stop function')

    def client_response(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_response function')

    def client_ready(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_ready function')

    def client_ready_block(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_ready_block function')

    def client_start(__self__):
        raise NotImplementedError('subclass ClientControlService and implement your own client_start function')

class ClientControlProxy(object):
    def __init__(self, clnt):
        self.__clnt__ = clnt

    def async_client_get_txn_names(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_GET_TXN_NAMES, [], ClientControlService.__input_type_info__['client_get_txn_names'], ClientControlService.__output_type_info__['client_get_txn_names'])

    def async_client_shutdown(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_SHUTDOWN, [], ClientControlService.__input_type_info__['client_shutdown'], ClientControlService.__output_type_info__['client_shutdown'])

    def async_client_force_stop(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_FORCE_STOP, [], ClientControlService.__input_type_info__['client_force_stop'], ClientControlService.__output_type_info__['client_force_stop'])

    def async_client_response(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_RESPONSE, [], ClientControlService.__input_type_info__['client_response'], ClientControlService.__output_type_info__['client_response'])

    def async_client_ready(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_READY, [], ClientControlService.__input_type_info__['client_ready'], ClientControlService.__output_type_info__['client_ready'])

    def async_client_ready_block(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_READY_BLOCK, [], ClientControlService.__input_type_info__['client_ready_block'], ClientControlService.__output_type_info__['client_ready_block'])

    def async_client_start(__self__):
        return __self__.__clnt__.async_call(ClientControlService.CLIENT_START, [], ClientControlService.__input_type_info__['client_start'], ClientControlService.__output_type_info__['client_start'])

    def sync_client_get_txn_names(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_GET_TXN_NAMES, [], ClientControlService.__input_type_info__['client_get_txn_names'], ClientControlService.__output_type_info__['client_get_txn_names'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_client_shutdown(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_SHUTDOWN, [], ClientControlService.__input_type_info__['client_shutdown'], ClientControlService.__output_type_info__['client_shutdown'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_client_force_stop(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_FORCE_STOP, [], ClientControlService.__input_type_info__['client_force_stop'], ClientControlService.__output_type_info__['client_force_stop'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_client_response(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_RESPONSE, [], ClientControlService.__input_type_info__['client_response'], ClientControlService.__output_type_info__['client_response'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_client_ready(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_READY, [], ClientControlService.__input_type_info__['client_ready'], ClientControlService.__output_type_info__['client_ready'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_client_ready_block(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_READY_BLOCK, [], ClientControlService.__input_type_info__['client_ready_block'], ClientControlService.__output_type_info__['client_ready_block'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

    def sync_client_start(__self__):
        __result__ = __self__.__clnt__.sync_call(ClientControlService.CLIENT_START, [], ClientControlService.__input_type_info__['client_start'], ClientControlService.__output_type_info__['client_start'])
        if __result__[0] != 0:
            raise Exception("RPC returned non-zero error code %d: %s" % (__result__[0], os.strerror(__result__[0])))
        if len(__result__[1]) == 1:
            return __result__[1][0]
        elif len(__result__[1]) > 1:
            return __result__[1]

