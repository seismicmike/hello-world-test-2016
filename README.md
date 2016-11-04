# Hello World Coding Test

This module was developed as a coding test for a job application in the fall of 2016.

Here were the instructions for the assigment:

# Instructions
Please read through this carefully before beginning development. We're looking for a hand-coded module to help us understand your familiarity with the Drupal 7 API as well as your creative problem solving skills. If you have any questions or if anything is unclear, please don't hesitate to reach out! Here are the requirements...

content_type_vocab_hello_world Module: <redacted>

Hello World Exercise
The goal of this exercise is for us to get to know how you go about programming a custom Drupal 7 module.


There are two items you'll send back to us:
* a zipped module folder
* a brief document describing the decision-making you used during the creation of the module.


## Step 1
Install and enable the above module (content_type_vocab_hello_world). It will create a taxonomy vocabulary called "Sections" and a content type called "Hello World Article" that you'll need to use.


## Step 2
Create the following terms in the Sections vocabulary:
* "About Us" - make sure it's Enabled box is checked.
* "Misc" - make sure it's Enabled box is checked.
* "News" - make sure it's Enabled box is unchecked.
Note, the module does not have to create these terms automatically.


## Step 3
Create 4 content nodes using the Hello World Article content type. Two nodes should be tagged with the "News" value only in the Sections field. The other two should be tagged once in the Sections field with "About Us" on one node and "Misc" on the last node. The module does not have to generate these nodes automatically.


## Step 4
Set Bartik as the default theme if it isn't set as the default already.


## Step 5
Read the requirements below...
The text "Hello World!" should appear in bold typeface within a block on the right side of all Hello World Article pages only.

A list of hyperlinked titles to all nodes that are of Hello World Article type, and are tagged with "Enabled" terms from the Sections vocabulary, should appear below the "Hello World!" text on Hello World Article pages.

When viewing a Hello World Article on the Drupal site, the phrase "Content starts here!" should appear in an italic typeface as the first line of content on the page.

Create a node access function to limit access to these nodes to only a logged in user.

Additionally, create a themeable page that contains a listing of all Hello World Article nodes in order of creation descending. The page should be reachable by the URL /hello-world. It must contain an edit and delete link to each of the nodes. - Themable meaning a theme function that can include or not include a template.

This page should include the following items: Title, Published Date, List of Sections, Full Body, Node Links (Edit/Delete)


All of this functionality needs to be contained in one Drupal module. The only module's dependencies should be Drupal core modules, Features, and the content_type_vocab_hello_world module.

Additionally, the Views module cannot be used for this exercise.
Name the module "hello_world".
Compatible with Drupal 7.


## Step 6
Briefly document how these feature will be built in written format, describing the thinking and approach to fulfilling the requirements. E-mail, Word or text file will suffice.


## Step 7
Write the module.


## Step 8
Once completed, zip the module folder and documentation, then send to <redacted>

Take up to 3 days to complete the assignment.

# My Technical Approach:
## Hello World Article List Block
### The Requirement
Two of the listed requirements will be served by creating a block to list links to the articles. Those two requirements are:
* The text "Hello World!" should appear in bold typeface within a block on the right side of all Hello World Article pages only.
* A list of hyperlinked titles to all nodes that are of Hello World Article type, and are tagged with "Enabled" terms from the Sections vocabulary, should appear below the "Hello World!" text on Hello World Article pages.

### My Solution
Obviously, the easiest way to do this would be to use views, but since views is not an allowable dependency, I’ll have to do this by hand.
To do this, I will create a block with the title “Hello World!” and a list of article titles and links to the titles. The content of the block will be a render array of type #item with #theme “item_list” so the content can be themed as a list.

To generate the content, I will query the database for nodes with an enabled section taxonomy tag. In order to do this, I will need to write a join. Something like the following ought to do the trick, which I can run with db_query:

```
SELECT
  s.entity_id
FROM {field_data_field_sections} s
JOIN {field_data_field_enabled} e ON s.field_sections_tid = e.entity_id
WHERE e.entity_type = 'taxonomy_term'
  AND s.entity_type = 'node'
  AND s.bundle = 'hello_world_article'
  AND e.field_enabled_value = 1)
```

The reason for writing the query this way rather than loading each node and inspecting these properties on each one-by-one should be obvious. It’s vastly more efficient to structure good queries to get what you need. If you have 100s or 1,000s of nodes to loop through, your site is going to crawl. These entity tables are well indexed, so this query should be fast, even if you have a ton of nodes.

Each item in the list will be a link to a Hello World article, which will be rendered using the l() function to generate the HTML link.
Controlling Visibility of the Block

I’m not entirely certain I’m satisfied with my solution to how I’m controlling placement and visibility of the block here. Of the possibilities, available to me, I wanted one that would not be dependent on configuration settings to be made in the user interface. I would normally look to Page Manager, Contexts or Features Extra for something like this, but those were not allowable modules.

So, what I did was set the region in the hook_block_info for this block. I also set ‘enabled’ to be true. This made the block visible on every page, so then to restrict it to just the node view pages, I inspected the current page in hook_page_build and if the page was not a node view, I removed the block from the region (and removed the region altogether if there were no other blocks in it).

My gut tells me there must be a better way to handle this. The weakness of it is that it does not allow the user to override this setting from the block configuration page. It would require code to change it. I’m also not entirely sure what would happen if the theme were changed to one that uses different region names. I expect the block would not show at all unless configured on the admin page.

## Prepend content to Hello World Node Pages
### The requirement:
* When viewing a Hello World Article on the Drupal site, the phrase "Content starts here!" should appear in an italic typeface as the first line of content on the page.

### My solution
I have two options that I can think of off the top of my head:

1. Create a custom block with the required text in it and place it in the Content region above the Main Page Content “block”, which I then limit to only nodes of type Hello World Article using the block config page.
Use hook_page_build to prepend a custom content item to the content region for Hello World Article node pages.
2. I would have a third option to use Page Manager to override the node view page with a custom variant for the Hello World content type wherein I would add a custom content item (pseudo block) to the content region, but since ctools, panels and page manager are not allowable dependencies, I can’t use this method.

I prefer #2 because it’s easier to encase in code rather than having to rely on on-page configuration. Features might help here, but I prefer not to use features whenever possible to simplify matters. (Don’t get me wrong. I use features A LOT - and I love it - but sometimes it can be a little unwieldy or unpredictable.)

To do this, I’ll need a hook_page_build. I will identify whether I am on a Hello World node page by using menu_get_object to see if I have a node, and then inspecting the ->type attribute of the node to see if I have a hello_world_article node.

Then to prepend my content, I’ll just add it to the $page[‘content’] array with a weight that should float it up to the top.

Styling it to be italic will be accomplished by wrapping it in an element with a class. Then I will use #attached to pull in a CSS file from my module that will style that class to be font-style italic.

There may be one potential flaw to this, and that is that the above method causes the “Content starts here!” text to appear before the attribution line. I felt that was an open matter of interpretation as to whether this satisfied the requirements. If this does not, I could change the approach slightly to prepend the paragraph to the actual body content itself if necessary.

## Custom Node Access to Restrict Hello World Nodes to Authenticated Users
### The Requirement
* Create a node access function to limit access to these nodes to only a logged in user.

### My Solution
This one’s easy. Implement hook_node_access. Check the $account to see if the user is anonymous or authenticated. If the $account->uid is 0, it’s anonymous. Deny access.

I also had to use hook_menu_alter to apply this access callback to the /node page so that the teasers didn’t show to anonymous users.

## Hello World Article List Page
### The Requirement
* Additionally, create a themeable page that contains a listing of all Hello World Article nodes in order of creation descending. The page should be reachable by the URL /hello-world. It must contain an edit and delete link to each of the nodes. - Themable meaning a theme function that can include or not include a template.
  - This page should include the following items: Title, Published Date, List of Sections, Full Body, Node Links (Edit/Delete)

### My Solution
Here again, the obviously easiest way to do this would be to use views, but since views is not an allowable dependency, I’ll have to do it by hand.

Create a page using hook_menu with a callback that will query for all nodes of type Hello World Article, ordered by publish date descending. This should be a pretty simple select query that can be run with db_query() .

The query should be:

```
SELECT nid
FROM {node}
WHERE type=”hello_world_article”
ORDER BY created DESC;
```

For each of the node IDs found by this query, I will load the node and then generate a theme_table row array with the required information.
* The title will be rendered using l() to link to the node.
* The Published Date will be rendered using format_date and the long date format
* The List of Sections will be rendered as an item_list with each item being a link rendered by the l() function to the Taxonomy vocabulary
* The Full Body will simply be a wall of text. At this point I am only supporting the plain text text format as the WYSIWYG module is not supported.
* The Edit and Delete links will be rendered in a single column called “Operations”. They will be rendered via the l() function and will link to the appropriate node operation for the content item.
The callback will return a render array with a content item of #type item and #theme table to be rendered as a table.
“Themability” was achieved by:
* Producing the content as a render array rather than HTML so that hook_page_build and hook_page_alter could manipulate it
* Having the table output using the standard theme_table, the links output using l(), and the lists output using theme_item_list, which can be altered if necessary
* Making heavy use of classes and ids for the table, link, and list elements.
