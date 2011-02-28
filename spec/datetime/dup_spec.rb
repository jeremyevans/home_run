require File.expand_path('../../spec_helper', __FILE__)

describe "DateTime#dup" do
  before do
    @d = DateTime.now
  end

  it "should return a copy of the date" do
    @d.dup.should == @d 
  end

  it "should return a different object_id" do
    @d.dup.object_id.should_not == @d.object_id
  end

  it "should not keep frozen status" do
    @d.freeze
    @d.dup.frozen?.should be_false
  end

  it "should not keep singleton_methods" do
    class << @d; def foo() 1 end end
    @d.dup.respond_to?(:foo).should be_false
  end
end
