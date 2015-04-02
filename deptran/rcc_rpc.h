#pragma once

#include "rrr.hpp"
#include "event/event.hpp"

#include <errno.h>


namespace rococo {

struct RequestHeader {
    rrr::i32 t_type;
    rrr::i32 p_type;
    rrr::i32 cid;
    rrr::i64 tid;
    rrr::i64 pid;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const RequestHeader& o) {
    m << o.t_type;
    m << o.p_type;
    m << o.cid;
    m << o.tid;
    m << o.pid;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, RequestHeader& o) {
    m >> o.t_type;
    m >> o.p_type;
    m >> o.cid;
    m >> o.tid;
    m >> o.pid;
    return m;
}

struct BatchRequestHeader {
    rrr::i32 t_type;
    rrr::i32 cid;
    rrr::i64 tid;
    rrr::i32 num_piece;
    rrr::i32 expected_output_size;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const BatchRequestHeader& o) {
    m << o.t_type;
    m << o.cid;
    m << o.tid;
    m << o.num_piece;
    m << o.expected_output_size;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, BatchRequestHeader& o) {
    m >> o.t_type;
    m >> o.cid;
    m >> o.tid;
    m >> o.num_piece;
    m >> o.expected_output_size;
    return m;
}

struct BatchChopStartResponse {
    std::vector<rrr::i8> is_defers;
    std::vector<std::vector<Value>> outputs;
    GraphMarshaler gra_m;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const BatchChopStartResponse& o) {
    m << o.is_defers;
    m << o.outputs;
    m << o.gra_m;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, BatchChopStartResponse& o) {
    m >> o.is_defers;
    m >> o.outputs;
    m >> o.gra_m;
    return m;
}

struct ChopStartResponse {
    rrr::i8 is_defered;
    std::vector<Value> output;
    GraphMarshaler gra_m;
    std::string read_only;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ChopStartResponse& o) {
    m << o.is_defered;
    m << o.output;
    m << o.gra_m;
    m << o.read_only;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ChopStartResponse& o) {
    m >> o.is_defered;
    m >> o.output;
    m >> o.gra_m;
    m >> o.read_only;
    return m;
}

struct ChopFinishRequest {
    rrr::i64 txn_id;
    Graph<TxnInfo> gra;
    std::string read_only;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ChopFinishRequest& o) {
    m << o.txn_id;
    m << o.gra;
    m << o.read_only;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ChopFinishRequest& o) {
    m >> o.txn_id;
    m >> o.gra;
    m >> o.read_only;
    return m;
}

struct ChopFinishResponse {
    rrr::i32 res;
    std::vector<std::pair<RequestHeader, std::vector<Value>>> outputs;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ChopFinishResponse& o) {
    m << o.res;
    m << o.outputs;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ChopFinishResponse& o) {
    m >> o.res;
    m >> o.outputs;
    return m;
}

struct CollectFinishResponse {
    GraphMarshaler gra_m;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const CollectFinishResponse& o) {
    m << o.gra_m;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, CollectFinishResponse& o) {
    m >> o.gra_m;
    return m;
}

struct ValueTimesPair {
    rrr::i64 value;
    rrr::i64 times;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ValueTimesPair& o) {
    m << o.value;
    m << o.times;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ValueTimesPair& o) {
    m >> o.value;
    m >> o.times;
    return m;
}

struct TxnInfoRes {
    rrr::i32 start_txn;
    rrr::i32 total_txn;
    rrr::i32 total_try;
    rrr::i32 commit_txn;
    std::vector<double> this_latency;
    std::vector<double> last_latency;
    std::vector<double> attempt_latency;
    std::vector<double> interval_latency;
    std::vector<rrr::i32> num_try;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const TxnInfoRes& o) {
    m << o.start_txn;
    m << o.total_txn;
    m << o.total_try;
    m << o.commit_txn;
    m << o.this_latency;
    m << o.last_latency;
    m << o.attempt_latency;
    m << o.interval_latency;
    m << o.num_try;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, TxnInfoRes& o) {
    m >> o.start_txn;
    m >> o.total_txn;
    m >> o.total_try;
    m >> o.commit_txn;
    m >> o.this_latency;
    m >> o.last_latency;
    m >> o.attempt_latency;
    m >> o.interval_latency;
    m >> o.num_try;
    return m;
}

struct ServerResponse {
    std::map<std::string, ValueTimesPair> statistics;
    double cpu_util;
    rrr::i64 r_cnt_sum;
    rrr::i64 r_cnt_num;
    rrr::i64 r_sz_sum;
    rrr::i64 r_sz_num;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ServerResponse& o) {
    m << o.statistics;
    m << o.cpu_util;
    m << o.r_cnt_sum;
    m << o.r_cnt_num;
    m << o.r_sz_sum;
    m << o.r_sz_num;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ServerResponse& o) {
    m >> o.statistics;
    m >> o.cpu_util;
    m >> o.r_cnt_sum;
    m >> o.r_cnt_num;
    m >> o.r_sz_sum;
    m >> o.r_sz_num;
    return m;
}

struct ClientResponse {
    std::map<rrr::i32, TxnInfoRes> txn_info;
    rrr::i64 run_sec;
    rrr::i64 run_nsec;
    rrr::i64 period_sec;
    rrr::i64 period_nsec;
    rrr::i32 is_finish;
    rrr::i64 n_asking;
};

inline rrr::Marshal& operator <<(rrr::Marshal& m, const ClientResponse& o) {
    m << o.txn_info;
    m << o.run_sec;
    m << o.run_nsec;
    m << o.period_sec;
    m << o.period_nsec;
    m << o.is_finish;
    m << o.n_asking;
    return m;
}

inline rrr::Marshal& operator >>(rrr::Marshal& m, ClientResponse& o) {
    m >> o.txn_info;
    m >> o.run_sec;
    m >> o.run_nsec;
    m >> o.period_sec;
    m >> o.period_nsec;
    m >> o.is_finish;
    m >> o.n_asking;
    return m;
}

class RococoService: public rrr::Service {
public:
    enum {
        START_PIE = 0x3a743129,
        PREPARE_TXN = 0x1b0a7970,
        COMMIT_TXN = 0x24e1ca21,
        ABORT_TXN = 0x1709d704,
        BATCH_START_PIE = 0x5b5f4cfd,
        NAIVE_BATCH_START_PIE = 0x28bb75e4,
        RCC_START_PIE = 0x3cfba832,
        RCC_FINISH_TXN = 0x3f6b3d9f,
        RCC_ASK_TXN = 0x43d03e15,
        RCC_RO_START_PIE = 0x1bd0ee8d,
        RCC_BATCH_START_PIE = 0x220786b9,
        RPC_NULL = 0x28b9b19b,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(START_PIE, this, &RococoService::__start_pie__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(PREPARE_TXN, this, &RococoService::__prepare_txn__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(COMMIT_TXN, this, &RococoService::__commit_txn__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(ABORT_TXN, this, &RococoService::__abort_txn__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(BATCH_START_PIE, this, &RococoService::__batch_start_pie__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(NAIVE_BATCH_START_PIE, this, &RococoService::__naive_batch_start_pie__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCC_START_PIE, this, &RococoService::__rcc_start_pie__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCC_FINISH_TXN, this, &RococoService::__rcc_finish_txn__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCC_ASK_TXN, this, &RococoService::__rcc_ask_txn__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCC_RO_START_PIE, this, &RococoService::__rcc_ro_start_pie__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RCC_BATCH_START_PIE, this, &RococoService::__rcc_batch_start_pie__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(RPC_NULL, this, &RococoService::__rpc_null__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(START_PIE);
        svr->unreg(PREPARE_TXN);
        svr->unreg(COMMIT_TXN);
        svr->unreg(ABORT_TXN);
        svr->unreg(BATCH_START_PIE);
        svr->unreg(NAIVE_BATCH_START_PIE);
        svr->unreg(RCC_START_PIE);
        svr->unreg(RCC_FINISH_TXN);
        svr->unreg(RCC_ASK_TXN);
        svr->unreg(RCC_RO_START_PIE);
        svr->unreg(RCC_BATCH_START_PIE);
        svr->unreg(RPC_NULL);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual coro_f(start_pie, const RequestHeader& header, const std::vector<Value>& input, const rrr::i32& output_size, rrr::i32* res, std::vector<Value>* output, rrr::DeferredReply* defer) = 0;
    virtual coro_f(prepare_txn, const rrr::i64& tid, const std::vector<rrr::i32>& sids, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( commit_txn, const rrr::i64& tid, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( abort_txn, const rrr::i64& tid, rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( batch_start_pie, const BatchRequestHeader& batch_header, const std::vector<Value>& input, rrr::i32* res, std::vector<Value>* output) = 0;
    virtual coro_f( naive_batch_start_pie, const std::vector<RequestHeader>& header, const std::vector<std::vector<Value>>& input, const std::vector<rrr::i32>& output_size, std::vector<rrr::i32>* res, std::vector<std::vector<Value>>* output, rrr::DeferredReply* defer) = 0;
    virtual coro_f( rcc_start_pie, const RequestHeader& header, const std::vector<Value>& input, ChopStartResponse* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( rcc_finish_txn, const ChopFinishRequest& req, ChopFinishResponse* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( rcc_ask_txn, const rrr::i64& txn_id, CollectFinishResponse* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( rcc_ro_start_pie, const RequestHeader& header, const std::vector<Value>& input, std::vector<Value>* output, rrr::DeferredReply* defer) = 0;
    virtual coro_f( rcc_batch_start_pie, const std::vector<RequestHeader>& headers, const std::vector<std::vector<Value>>& inputs, BatchChopStartResponse* res, rrr::DeferredReply* defer) = 0;
    virtual coro_f( rpc_null, rrr::DeferredReply* defer) = 0;
private:
    void __start_pie__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        RequestHeader* in_0 = new RequestHeader;
        req->m >> *in_0;
        std::vector<Value>* in_1 = new std::vector<Value>;
        req->m >> *in_1;
        rrr::i32* in_2 = new rrr::i32;
        req->m >> *in_2;
        rrr::i32* out_0 = new rrr::i32;
        std::vector<Value>* out_1 = new std::vector<Value>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);

        auto f = boost::bind(&RococoService::start_pie, this, _1, boost::ref(*in_0), boost::ref(*in_1), boost::ref(*in_2), out_0, out_1, __defer__);
        //coro_t c(f);
        rrr::Coroutine::mkcoroutine(f);
        //this->start_pie(*in_0, *in_1, *in_2, out_0, out_1, __defer__);
    }
    void __prepare_txn__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        std::vector<rrr::i32>* in_1 = new std::vector<rrr::i32>;
        req->m >> *in_1;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);

        auto f = boost::bind(&RococoService::prepare_txn, this, _1, boost::ref(*in_0), boost::ref(*in_1), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->prepare_txn(*in_0, *in_1, out_0, __defer__);
    }
    void __commit_txn__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);

        auto f = boost::bind(&RococoService::commit_txn, this, _1, boost::ref(*in_0), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->commit_txn(*in_0, out_0, __defer__);
    }
    void __abort_txn__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);

        auto f = boost::bind(&RococoService::abort_txn, this, _1, boost::ref(*in_0), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->abort_txn(*in_0, out_0, __defer__);
    }
    void __batch_start_pie__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        BatchRequestHeader in_0;
        req->m >> in_0;
        std::vector<Value> in_1;
        req->m >> in_1;
        rrr::i32 out_0;
        std::vector<Value> out_1;

        auto f = boost::bind(&RococoService::batch_start_pie, this, _1, boost::ref(in_0), boost::ref(in_1), &out_0, &out_1);
        rrr::Coroutine::mkcoroutine(f);
        //this->batch_start_pie(in_0, in_1, &out_0, &out_1);
        sconn->begin_reply(req);
        *sconn << out_0;
        *sconn << out_1;
        sconn->end_reply();
        delete req;
        sconn->release();
    }
    void __naive_batch_start_pie__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::vector<RequestHeader>* in_0 = new std::vector<RequestHeader>;
        req->m >> *in_0;
        std::vector<std::vector<Value>>* in_1 = new std::vector<std::vector<Value>>;
        req->m >> *in_1;
        std::vector<rrr::i32>* in_2 = new std::vector<rrr::i32>;
        req->m >> *in_2;
        std::vector<rrr::i32>* out_0 = new std::vector<rrr::i32>;
        std::vector<std::vector<Value>>* out_1 = new std::vector<std::vector<Value>>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
            *sconn << *out_1;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete in_2;
            delete out_0;
            delete out_1;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        
        auto f = boost::bind(&RococoService::naive_batch_start_pie, this, _1, boost::ref(*in_0), boost::ref(*in_1), boost::ref(*in_2), out_0, out_1, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->naive_batch_start_pie(*in_0, *in_1, *in_2, out_0, out_1, __defer__);
    }
    void __rcc_start_pie__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        RequestHeader* in_0 = new RequestHeader;
        req->m >> *in_0;
        std::vector<Value>* in_1 = new std::vector<Value>;
        req->m >> *in_1;
        ChopStartResponse* out_0 = new ChopStartResponse;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);

        auto f = boost::bind(&RococoService::rcc_start_pie, this, _1, boost::ref(*in_0), boost::ref(*in_1), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->rcc_start_pie(*in_0, *in_1, out_0, __defer__);
    }
    void __rcc_finish_txn__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        ChopFinishRequest* in_0 = new ChopFinishRequest;
        req->m >> *in_0;
        ChopFinishResponse* out_0 = new ChopFinishResponse;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        
        auto f = boost::bind(&RococoService::rcc_finish_txn, this, _1, boost::ref(*in_0), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->rcc_finish_txn(*in_0, out_0, __defer__);
    }
    void __rcc_ask_txn__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i64* in_0 = new rrr::i64;
        req->m >> *in_0;
        CollectFinishResponse* out_0 = new CollectFinishResponse;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        
        auto f = boost::bind(&RococoService::rcc_ask_txn, this, _1, boost::ref(*in_0), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->rcc_ask_txn(*in_0, out_0, __defer__);
    }
    void __rcc_ro_start_pie__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        RequestHeader* in_0 = new RequestHeader;
        req->m >> *in_0;
        std::vector<Value>* in_1 = new std::vector<Value>;
        req->m >> *in_1;
        std::vector<Value>* out_0 = new std::vector<Value>;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        
        auto f = boost::bind(&RococoService::rcc_ro_start_pie, this, _1, boost::ref(*in_0), boost::ref(*in_1), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->rcc_ro_start_pie(*in_0, *in_1, out_0, __defer__);
    }
    void __rcc_batch_start_pie__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        std::vector<RequestHeader>* in_0 = new std::vector<RequestHeader>;
        req->m >> *in_0;
        std::vector<std::vector<Value>>* in_1 = new std::vector<std::vector<Value>>;
        req->m >> *in_1;
        BatchChopStartResponse* out_0 = new BatchChopStartResponse;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete in_0;
            delete in_1;
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
            
        auto f = boost::bind(&RococoService::rcc_batch_start_pie, this, _1, boost::ref(*in_0), boost::ref(*in_1), out_0, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->rcc_batch_start_pie(*in_0, *in_1, out_0, __defer__);
    }
    void __rpc_null__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto __marshal_reply__ = [=] {
        };
        auto __cleanup__ = [=] {
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        
        auto f = boost::bind(&RococoService::rpc_null, this, _1, __defer__);
        rrr::Coroutine::mkcoroutine(f);
        //this->rpc_null(__defer__);
    }
};

class RococoProxy {
protected:
    rrr::Client* __cl__;
public:
    RococoProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_start_pie(const RequestHeader& header, const std::vector<Value>& input, const rrr::i32& output_size, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::START_PIE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << header;
            *__cl__ << input;
            *__cl__ << output_size;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 start_pie(const RequestHeader& header, const std::vector<Value>& input, const rrr::i32& output_size, rrr::i32* res, std::vector<Value>* output) {
        rrr::Future* __fu__ = this->async_start_pie(header, input, output_size);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_prepare_txn(const rrr::i64& tid, const std::vector<rrr::i32>& sids, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::PREPARE_TXN, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
            *__cl__ << sids;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 prepare_txn(const rrr::i64& tid, const std::vector<rrr::i32>& sids, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_prepare_txn(tid, sids);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_commit_txn(const rrr::i64& tid, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::COMMIT_TXN, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 commit_txn(const rrr::i64& tid, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_commit_txn(tid);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_abort_txn(const rrr::i64& tid, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::ABORT_TXN, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << tid;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 abort_txn(const rrr::i64& tid, rrr::i32* res) {
        rrr::Future* __fu__ = this->async_abort_txn(tid);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_batch_start_pie(const BatchRequestHeader& batch_header, const std::vector<Value>& input, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::BATCH_START_PIE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << batch_header;
            *__cl__ << input;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 batch_start_pie(const BatchRequestHeader& batch_header, const std::vector<Value>& input, rrr::i32* res, std::vector<Value>* output) {
        rrr::Future* __fu__ = this->async_batch_start_pie(batch_header, input);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_naive_batch_start_pie(const std::vector<RequestHeader>& header, const std::vector<std::vector<Value>>& input, const std::vector<rrr::i32>& output_size, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::NAIVE_BATCH_START_PIE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << header;
            *__cl__ << input;
            *__cl__ << output_size;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 naive_batch_start_pie(const std::vector<RequestHeader>& header, const std::vector<std::vector<Value>>& input, const std::vector<rrr::i32>& output_size, std::vector<rrr::i32>* res, std::vector<std::vector<Value>>* output) {
        rrr::Future* __fu__ = this->async_naive_batch_start_pie(header, input, output_size);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rcc_start_pie(const RequestHeader& header, const std::vector<Value>& input, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::RCC_START_PIE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << header;
            *__cl__ << input;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rcc_start_pie(const RequestHeader& header, const std::vector<Value>& input, ChopStartResponse* res) {
        rrr::Future* __fu__ = this->async_rcc_start_pie(header, input);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rcc_finish_txn(const ChopFinishRequest& req, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::RCC_FINISH_TXN, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << req;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rcc_finish_txn(const ChopFinishRequest& req, ChopFinishResponse* res) {
        rrr::Future* __fu__ = this->async_rcc_finish_txn(req);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rcc_ask_txn(const rrr::i64& txn_id, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::RCC_ASK_TXN, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << txn_id;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rcc_ask_txn(const rrr::i64& txn_id, CollectFinishResponse* res) {
        rrr::Future* __fu__ = this->async_rcc_ask_txn(txn_id);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rcc_ro_start_pie(const RequestHeader& header, const std::vector<Value>& input, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::RCC_RO_START_PIE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << header;
            *__cl__ << input;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rcc_ro_start_pie(const RequestHeader& header, const std::vector<Value>& input, std::vector<Value>* output) {
        rrr::Future* __fu__ = this->async_rcc_ro_start_pie(header, input);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *output;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rcc_batch_start_pie(const std::vector<RequestHeader>& headers, const std::vector<std::vector<Value>>& inputs, const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::RCC_BATCH_START_PIE, __fu_attr__);
        if (__fu__ != nullptr) {
            *__cl__ << headers;
            *__cl__ << inputs;
        }
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rcc_batch_start_pie(const std::vector<RequestHeader>& headers, const std::vector<std::vector<Value>>& inputs, BatchChopStartResponse* res) {
        rrr::Future* __fu__ = this->async_rcc_batch_start_pie(headers, inputs);
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_rpc_null(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(RococoService::RPC_NULL, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 rpc_null() {
        rrr::Future* __fu__ = this->async_rpc_null();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class ServerControlService: public rrr::Service {
public:
    enum {
        SERVER_SHUTDOWN = 0x18dbfed8,
        SERVER_READY = 0x31436c6d,
        SERVER_HEART_BEAT_WITH_DATA = 0x59acd9da,
        SERVER_HEART_BEAT = 0x3cd34ed0,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(SERVER_SHUTDOWN, this, &ServerControlService::__server_shutdown__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SERVER_READY, this, &ServerControlService::__server_ready__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SERVER_HEART_BEAT_WITH_DATA, this, &ServerControlService::__server_heart_beat_with_data__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(SERVER_HEART_BEAT, this, &ServerControlService::__server_heart_beat__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(SERVER_SHUTDOWN);
        svr->unreg(SERVER_READY);
        svr->unreg(SERVER_HEART_BEAT_WITH_DATA);
        svr->unreg(SERVER_HEART_BEAT);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void server_shutdown() = 0;
    virtual void server_ready(rrr::i32* res) = 0;
    virtual void server_heart_beat_with_data(ServerResponse* res) = 0;
    virtual void server_heart_beat() = 0;
private:
    void __server_shutdown__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            this->server_shutdown();
            sconn->begin_reply(req);
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __server_ready__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            rrr::i32 out_0;
            this->server_ready(&out_0);
            sconn->begin_reply(req);
            *sconn << out_0;
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __server_heart_beat_with_data__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            ServerResponse out_0;
            this->server_heart_beat_with_data(&out_0);
            sconn->begin_reply(req);
            *sconn << out_0;
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __server_heart_beat__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            this->server_heart_beat();
            sconn->begin_reply(req);
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
};

class ServerControlProxy {
protected:
    rrr::Client* __cl__;
public:
    ServerControlProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_server_shutdown(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_SHUTDOWN, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_shutdown() {
        rrr::Future* __fu__ = this->async_server_shutdown();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_server_ready(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_READY, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_ready(rrr::i32* res) {
        rrr::Future* __fu__ = this->async_server_ready();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_server_heart_beat_with_data(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_HEART_BEAT_WITH_DATA, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_heart_beat_with_data(ServerResponse* res) {
        rrr::Future* __fu__ = this->async_server_heart_beat_with_data();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_server_heart_beat(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ServerControlService::SERVER_HEART_BEAT, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 server_heart_beat() {
        rrr::Future* __fu__ = this->async_server_heart_beat();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

class ClientControlService: public rrr::Service {
public:
    enum {
        CLIENT_GET_TXN_NAMES = 0x60cdbcc4,
        CLIENT_SHUTDOWN = 0x3480baca,
        CLIENT_FORCE_STOP = 0x19a6b162,
        CLIENT_RESPONSE = 0x661cb1ac,
        CLIENT_READY = 0x6e321e29,
        CLIENT_READY_BLOCK = 0x48a23cb9,
        CLIENT_START = 0x56d10dff,
    };
    int __reg_to__(rrr::Server* svr) {
        int ret = 0;
        if ((ret = svr->reg(CLIENT_GET_TXN_NAMES, this, &ClientControlService::__client_get_txn_names__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_SHUTDOWN, this, &ClientControlService::__client_shutdown__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_FORCE_STOP, this, &ClientControlService::__client_force_stop__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_RESPONSE, this, &ClientControlService::__client_response__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_READY, this, &ClientControlService::__client_ready__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_READY_BLOCK, this, &ClientControlService::__client_ready_block__wrapper__)) != 0) {
            goto err;
        }
        if ((ret = svr->reg(CLIENT_START, this, &ClientControlService::__client_start__wrapper__)) != 0) {
            goto err;
        }
        return 0;
    err:
        svr->unreg(CLIENT_GET_TXN_NAMES);
        svr->unreg(CLIENT_SHUTDOWN);
        svr->unreg(CLIENT_FORCE_STOP);
        svr->unreg(CLIENT_RESPONSE);
        svr->unreg(CLIENT_READY);
        svr->unreg(CLIENT_READY_BLOCK);
        svr->unreg(CLIENT_START);
        return ret;
    }
    // these RPC handler functions need to be implemented by user
    // for 'raw' handlers, remember to reply req, delete req, and sconn->release(); use sconn->run_async for heavy job
    virtual void client_get_txn_names(std::map<rrr::i32, std::string>* txn_names) = 0;
    virtual void client_shutdown() = 0;
    virtual void client_force_stop() = 0;
    virtual void client_response(ClientResponse* res) = 0;
    virtual void client_ready(rrr::i32* res) = 0;
    virtual void client_ready_block(rrr::i32* res, rrr::DeferredReply* defer) = 0;
    virtual void client_start() = 0;
private:
    void __client_get_txn_names__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            std::map<rrr::i32, std::string> out_0;
            this->client_get_txn_names(&out_0);
            sconn->begin_reply(req);
            *sconn << out_0;
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __client_shutdown__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            this->client_shutdown();
            sconn->begin_reply(req);
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __client_force_stop__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            this->client_force_stop();
            sconn->begin_reply(req);
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __client_response__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            ClientResponse out_0;
            this->client_response(&out_0);
            sconn->begin_reply(req);
            *sconn << out_0;
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __client_ready__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            rrr::i32 out_0;
            this->client_ready(&out_0);
            sconn->begin_reply(req);
            *sconn << out_0;
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
    void __client_ready_block__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        rrr::i32* out_0 = new rrr::i32;
        auto __marshal_reply__ = [=] {
            *sconn << *out_0;
        };
        auto __cleanup__ = [=] {
            delete out_0;
        };
        rrr::DeferredReply* __defer__ = new rrr::DeferredReply(req, sconn, __marshal_reply__, __cleanup__);
        this->client_ready_block(out_0, __defer__);
    }
    void __client_start__wrapper__(rrr::Request* req, rrr::ServerConnection* sconn) {
        auto f = [=] {
            this->client_start();
            sconn->begin_reply(req);
            sconn->end_reply();
            delete req;
            sconn->release();
        };
        sconn->run_async(f);
    }
};

class ClientControlProxy {
protected:
    rrr::Client* __cl__;
public:
    ClientControlProxy(rrr::Client* cl): __cl__(cl) { }
    rrr::Future* async_client_get_txn_names(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_GET_TXN_NAMES, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_get_txn_names(std::map<rrr::i32, std::string>* txn_names) {
        rrr::Future* __fu__ = this->async_client_get_txn_names();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *txn_names;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_shutdown(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_SHUTDOWN, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_shutdown() {
        rrr::Future* __fu__ = this->async_client_shutdown();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_force_stop(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_FORCE_STOP, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_force_stop() {
        rrr::Future* __fu__ = this->async_client_force_stop();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_response(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_RESPONSE, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_response(ClientResponse* res) {
        rrr::Future* __fu__ = this->async_client_response();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_ready(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_READY, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_ready(rrr::i32* res) {
        rrr::Future* __fu__ = this->async_client_ready();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_ready_block(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_READY_BLOCK, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_ready_block(rrr::i32* res) {
        rrr::Future* __fu__ = this->async_client_ready_block();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        if (__ret__ == 0) {
            __fu__->get_reply() >> *res;
        }
        __fu__->release();
        return __ret__;
    }
    rrr::Future* async_client_start(const rrr::FutureAttr& __fu_attr__ = rrr::FutureAttr()) {
        rrr::Future* __fu__ = __cl__->begin_request(ClientControlService::CLIENT_START, __fu_attr__);
        __cl__->end_request();
        return __fu__;
    }
    rrr::i32 client_start() {
        rrr::Future* __fu__ = this->async_client_start();
        if (__fu__ == nullptr) {
            return ENOTCONN;
        }
        rrr::i32 __ret__ = __fu__->get_error_code();
        __fu__->release();
        return __ret__;
    }
};

} // namespace rococo



