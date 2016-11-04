<?php

/**
 * @file
 * Hello World Page.
 */

/**
 * Page callback for Hello World.
 *
 * @return array
 *   A render array.
 */
function hello_world_page() {
  $nodes = hello_world_get_all_article_nodes();
  $rows = [];

  foreach ($nodes as $node_id) {
    $node = node_load($node_id);

    $terms = $node->field_sections[LANGUAGE_NONE];
    $sections = [];

    foreach ($terms as $term) {
      $term = taxonomy_term_load($term['tid']);
      $sections[] = l($term->name, 'taxonomy/term/' . $term->tid, [
        'attributes' => [
          'class' => ['hello-world-section-link'],
        ],
      ]);
    }

    $rows[] = [
      [
        'data' => l($node->title, 'node/' . $node->nid, [
          'attributes' => ['class' => ['hello-world-article-title-link']],
        ]),
        'class' => [
          'hello-world-table-cell',
          'hello-world-table-cell-title',
        ],
      ],
      [
        'data' => format_date($node->created, 'long'),
        'class' => [
          'hello-world-table-cell',
          'hello-world-table-cell-published',
        ],
      ],
      [
        'data' => theme('item_list', [
          'type' => 'ul',
          'items' => $sections,
          'attributes' => [
            'class' => ['hello-world-sections-list'],
          ],
        ]),
        'class' => [
          'hello-world-table-cell',
          'hello-world-table-cell-sections',
        ],
      ],
      [
        'data' => $node->body[LANGUAGE_NONE][0]['value'],
        'class' => [
          'hello-world-table-cell',
          'hello-world-table-cell-body',
        ],
      ],
      [
        'data' => theme('item_list', [
          'type' => 'ul',
          'items' => [
            l(t('Edit'), 'node/' . $node->nid . '/edit', [
              'attributes' => [
                'class' => [
                  'hello-world-operations-link',
                  'hello-world-operations-edit',
                ],
              ],
            ]),
            l(t('Delete'), 'node/' . $node->nid . '/delete', [
              'attributes' => [
                'class' => [
                  'hello-world-operations-link',
                  'hello-world-operations-delete',
                ],
              ],
            ]),
          ],
          'attributes' => [
            'class' => ['hello-world-article-operations-link-list'],
          ],
        ]),
        'class' => [
          'hello-world-table-cell',
          'hello-world-table-cell-operations',
        ],
      ],
    ];
  }

  return [
    'hello_world_table' => [
      '#theme' => 'table',
      '#header' => [
        'Title',
        'Published Date',
        'Section(s)',
        'Body',
        'Operations',
      ],
      '#rows' => $rows,
      '#attributes' => [
        'id' => 'hello-world-articles-table',
        'class' => ['hello-world-table'],
      ],
      '#caption' => '',
      '#colgroups' => [],
      '#sticky' => TRUE,
      '#empty' => 'There is nothing in this table',
    ],
  ];
}
