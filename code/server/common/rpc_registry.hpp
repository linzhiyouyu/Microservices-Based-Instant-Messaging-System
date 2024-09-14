namespace lcz_zpy_im {
    class Registrar {
    public:
        using ptr = std::shared_ptr<Registrar>;
        Registrar(const std::string &registry_host): 
            _registry_host(registry_host),
            _etcd_client(new etcd::Client(registry_host)),
            _keepalive(_etcd_client->leasekeepalive(_lease_time_sec).get()),
            _lease_id(_keepalive->Lease()){
        }
        ~Registrar() {
            _etcd_client->leaserevoke(_lease_id);
            _keepalive->Cancel();
        }
        void registry(const std::string &service_host, const std::string &service_name) {
            auto resp = _etcd_client->put(service_name, service_host, _lease_id); //注册服务
            auto rsp = resp.get();
            if (rsp.is_ok() == false){
                ERROR("{} - {} 服务注册失败！", service_name, service_host);
            }
            DEBUG("{} - {} 服务注册成功！", service_name, service_host);
        }
        void cancel() {
            _etcd_client->leaserevoke(_lease_id);
        }
    private:
        const size_t _lease_time_sec = 3; //租约超时时间
        std::string _registry_host; //注册中心地址 "127.0.0.1:9090"
        std::unique_ptr<etcd::Client> _etcd_client;
        std::shared_ptr<etcd::KeepAlive> _keepalive;
        int64_t _lease_id;
    };
}