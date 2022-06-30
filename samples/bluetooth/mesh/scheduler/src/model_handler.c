#include <zephyr/bluetooth/bluetooth.h>
#include <bluetooth/mesh/models.h>
#include <dk_buttons_and_leds.h>
#include "model_handler.h"

static void attention_on(struct bt_mesh_model *mod)
{
	printk("Attention On\n");
}

static void attention_off(struct bt_mesh_model *mod)
{
	printk("Attention Off\n");
}

static const struct bt_mesh_health_srv_cb health_srv_cb = {
	.attn_on = attention_on,
	.attn_off = attention_off,
};

static struct bt_mesh_health_srv health_srv = {
	.cb = &health_srv_cb,
};

BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

static struct bt_mesh_time_srv time_srv = BT_MESH_TIME_SRV_INIT(NULL); // TODO verify parameters

static struct bt_mesh_scheduler_srv scheduler_srv = 
	BT_MESH_SCHEDULER_SRV_INIT(0xF, &time_srv); // TODO verify parameters

static struct bt_mesh_scheduler_cli scheduler_cli; // TODO initialize?

static struct bt_mesh_elem elements[] = {
	// TODO verify elements
	BT_MESH_ELEM(1,
		BT_MESH_MODEL_LIST(
			BT_MESH_MODEL_CFG_SRV,
			BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
			BT_MESH_MODEL_TIME_SRV(&time_srv),
			BT_MESH_MODEL_SCHEDULER_SRV(&scheduler_srv)),
		BT_MESH_MODEL_NONE),
	BT_MESH_ELEM(2,
		BT_MESH_MODEL_LIST(
			BT_MESH_MODEL_SCHEDULER_CLI(&scheduler_cli)),
		BT_MESH_MODEL_NONE),
};

static const struct bt_mesh_comp comp = {
	.cid = CONFIG_BT_COMPANY_ID,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

const struct bt_mesh_comp *model_handler_init(void)
{
	// TODO configure init
	return &comp;
}
