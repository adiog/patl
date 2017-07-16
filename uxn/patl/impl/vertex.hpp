/*-
 | This source code is part of PATL (Practical Algorithm Template Library)
 | Released under the BSD License (http://www.opensource.org/licenses/bsd-license.php)
 | Copyright (c) 2005, 2007..2009, Roman S. Klyujkov (uxnuxn AT gmail.com)
-*/
#ifndef PATL_IMPL_VERTEX_HPP
#define PATL_IMPL_VERTEX_HPP

namespace uxn
{
namespace patl
{
namespace impl
{

template <typename Algorithm>
class const_vertex_generic
    : public Algorithm
{
protected:
    typedef const_vertex_generic<Algorithm> this_t;
    typedef Algorithm algorithm;

public:
    typedef this_t const_vertex;
    typedef typename algorithm::cont_type cont_type;

protected:
    typedef typename algorithm::node_type node_type;
    typedef typename cont_type::bit_compare bit_compare;
    typedef typename cont_type::prefix prefix;
    typedef typename cont_type::const_preorder_iterator const_preorder_iterator;
    typedef typename cont_type::const_postorder_iterator const_postorder_iterator;
    typedef typename cont_type::preorder_iterator preorder_iterator;
    typedef typename cont_type::postorder_iterator postorder_iterator;
    typedef typename cont_type::const_iterator const_iterator;
    typedef typename cont_type::iterator iterator;
    typedef typename cont_type::const_reverse_iterator const_reverse_iterator;
    typedef typename cont_type::reverse_iterator reverse_iterator;

public:
    typedef typename algorithm::key_type key_type;
    typedef typename algorithm::const_key_reference const_key_reference;
    typedef typename algorithm::value_type value_type;

    explicit const_vertex_generic(const algorithm &pal = algorithm())
        : algorithm(pal)
    {
    }

    const_vertex_generic(const cont_type *cont, const node_type *q, word_t qid)
        : algorithm(cont, q, qid)
    {
    }

    const_vertex_generic(const cont_type *cont, word_t qq)
        : algorithm(cont, qq)
    {
    }

    bool operator<(const this_t &r) const
    {
        return compact() < r.compact();
    }

    prefix get_prefix() const
    {
        return prefix(cont(), get_q());
    }

    word_t node_q_uid() const
    {
        return reinterpret_cast<word_t>(get_q());
    }

    word_t node_p_uid() const
    {
        return reinterpret_cast<word_t>(get_p());
    }

    const_iterator begin() const
    {
        return const_iterator(*postorder_begin());
    }

    const_iterator end() const
    {
        return const_iterator(*postorder_end());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    const_postorder_iterator postorder_begin() const
    {
        this_t vtx(*this);
        if (compact())
            vtx.descend<0>();
        else
            vtx.toggle();
        return const_postorder_iterator(vtx);
    }

    const_postorder_iterator postorder_end() const
    {
        this_t vtx(*this);
        if (get_q())
            vtx.move<1>();
        else
            vtx.toggle();
        return const_postorder_iterator(vtx);
    }

    const_preorder_iterator preorder_begin() const
    {
        const node_type *q = get_q();
        return const_preorder_iterator(this_t(cont(), q, q ? get_qid() : 1));
    }
    const_preorder_iterator preorder_begin(word_t limit) const
    {
        this_t vtx(*this);
        if (compact())
            vtx.template descend<0>(limit);
        else
            vtx.toggle();
        return const_preorder_iterator(vtx);
    }

    const_preorder_iterator preorder_end() const
    {
        if (get_q())
        {
            const_preorder_iterator pit(*this);
            pit.next_subtree();
            return pit;
        }
        else
        {
            this_t vtx(*this);
            vtx.toggle();
            return const_preorder_iterator(vtx);
        }
    }

    const_key_reference parent_key() const
    {
        return get_key(get_q());
    }

    const value_type &parent_value() const
    {
        return get_value(get_q());
    }

    word_t skip() const
    {
        return get_q()->get_skip();
    }

    /// only for !leaf()
    word_t next_skip() const
    {
        return get_p()->get_skip();
    }

    bool limited(word_t limit) const
    {
        return get_qtag() || next_skip() >= limit;
    }

    /// trie descend to the leaves
    template <word_t Side>
    void descend()
    {
        while (!get_qtag())
            iterate(Side);
    }
    template <word_t Side, typename Callback>
    void descend(Callback &cb)
    {
        while (!get_qtag())
        {
            cb(this);
            iterate(Side);
        }
    }

    /// trie descend to the leaves limited by
    template <word_t Side>
    void descend(word_t limit)
    {
        while (!limited(limit))
            iterate(Side);
    }
    template <word_t Side, typename Callback>
    void descend(word_t limit, Callback &cb)
    {
        while (!limited(limit))
        {
            cb(this);
            iterate(Side);
        }
    }

    /// generic part of all move functions
    template <word_t Side>
    void move_subtree()
    {
        while (get_qid() == Side)
            ascend();
        toggle();
    }
    template <word_t Side, typename Callback>
    void move_subtree(Callback &cb)
    {
        while (get_qid() == Side)
        {
            ascend();
            cb(this);
        }
        toggle();
    }

    /// move to the next leaf
    template <word_t Side>
    void move()
    {
        move_subtree<Side>();
        descend<word_t(1) ^ Side>();
    }

    // low-level functions

    word_t get_parent_id() const
    {
        return get_q()->get_parent_id();
    }
};

template <typename Algorithm>
class vertex_generic
    : public const_vertex_generic<Algorithm>
{
protected:
    typedef const_vertex_generic<Algorithm> super;
    typedef vertex_generic<Algorithm> this_t;
    typedef Algorithm algorithm;

public:
    typedef super const_vertex;
    typedef typename algorithm::cont_type cont_type;

protected:
    typedef typename algorithm::node_type node_type;
    typedef typename cont_type::bit_compare bit_compare;
    typedef typename cont_type::prefix prefix;
    typedef typename cont_type::preorder_iterator preorder_iterator;
    typedef typename cont_type::postorder_iterator postorder_iterator;
    typedef typename cont_type::iterator iterator;
    typedef typename cont_type::reverse_iterator reverse_iterator;

public:
    typedef typename algorithm::key_type key_type;
    typedef typename algorithm::value_type value_type;

    explicit vertex_generic(const algorithm &pal = algorithm())
        : super(pal)
    {
    }

    vertex_generic(const cont_type *cont, const node_type *q, word_t qid)
        : super(cont, q, qid)
    {
    }

    vertex_generic(const cont_type *cont, word_t qq)
        : super(cont, qq)
    {
    }

    vertex_generic(const super &vtx)
        : super(vtx)
    {
    }

    operator const const_vertex&() const
    {
        return *this;
    }
    operator const_vertex&()
    {
        return *this;
    }

    iterator begin()
    {
        return iterator(*postorder_begin());
    }

    iterator end()
    {
        return iterator(*postorder_end());
    }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    postorder_iterator postorder_begin()
    {
        this_t vtx(*this);
        if (this->compact())
            vtx.template descend<0>();
        else
            vtx.toggle();
        return postorder_iterator(vtx);
    }

    postorder_iterator postorder_end()
    {
        this_t vtx(*this);
        if (get_q())
            vtx.move<1>();
        else
            vtx.toggle();
        return postorder_iterator(vtx);
    }

    preorder_iterator preorder_begin()
    {
        const node_type *q = get_q();
        return preorder_iterator(this_t(cont(), q, q ? get_qid() : 1));
    }
    preorder_iterator preorder_begin(word_t limit)
    {
        this_t vtx(*this);
        if (this->compact())
            vtx.template descend<0>(limit);
        else
            vtx.toggle();
        return preorder_iterator(vtx);
    }

    preorder_iterator preorder_end()
    {
        if (get_q())
        {
            preorder_iterator pit(*this);
            pit.next_subtree();
            return pit;
        }
        else
        {
            this_t vtx(*this);
            vtx.toggle();
            return preorder_iterator(vtx);
        }
    }, cur->key().substr(l

    value_type &parent_value()
    {
        return this->get_value(get_q());
    }
};

} // namespace impl
} // namespace patl
} // namespace uxn

#endif
