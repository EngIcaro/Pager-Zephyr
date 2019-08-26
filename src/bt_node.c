#include "bt_node.h"

/* Declare and Initialize Element Context */
struct onoff_state onoff_state = {
	.current    = OFF,
	.previous   = ON,
};

/* Server Configuration Declaration */
struct bt_mesh_cfg_srv cfg_srv = {
	.relay              = BT_MESH_RELAY_ENABLED,
	.beacon             = BT_MESH_BEACON_DISABLED,
	.frnd               = BT_MESH_FRIEND_NOT_SUPPORTED,
	.gatt_proxy         = BT_MESH_GATT_PROXY_ENABLED,
	.default_ttl        = 7,
	.net_transmit       = BT_MESH_TRANSMIT(1, 20),
	.relay_retransmit   = BT_MESH_TRANSMIT(1, 20),
};

/* Health Server Declaration */
static struct bt_mesh_health_srv health_srv = {};

/* Client Configuration Declaration */
static struct bt_mesh_cfg_cli cfg_cli = {};

/* Publication Declarations */
BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_srv, NULL, 2 + 2);

/* OnOff Model Server Op Dispatch Table */
const struct bt_mesh_model_op gen_onoff_srv_op[] = {
	{BT_MESH_MODEL_OP_GEN_ONOFF_GET, 0, gen_onoff_get},
	{BT_MESH_MODEL_OP_GEN_ONOFF_SET, 2, gen_onoff_set},
	{BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK, 2, gen_onoff_set_unack},
	BT_MESH_MODEL_OP_END,
};

/* Element Model Declaration */
struct bt_mesh_model root_models[] = {
	BT_MESH_MODEL_CFG_SRV(&cfg_srv),
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
	BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op, 
                  &gen_onoff_pub_srv, &onoff_state),
};

/* Root Element Declaration */
struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
};

/* Node Declaration */
const struct bt_mesh_comp comp = {
	.cid        = BT_COMP_ID_LF,
	.elem       = elements,
	.elem_count = ARRAY_SIZE(elements),
};